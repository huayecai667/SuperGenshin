#include "player.h"
#include "enemy.h"
#include "fireball.h"
#include "flag.h"
#include "piranhaplant.h"
#include "soundmanager.h"
#include "staticcoin.h"
#include <QGraphicsScene>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QKeyEvent>

long Player::m_score=0;
int Player::m_coin=0;

Player::Player(QGraphicsItem *parent)
    : QObject(),
    DynamicObject(parent),
    m_isLeftPressed(false),
    m_isRightPressed(false),
    m_isSquatPressed(false),
    m_isDead(false),
    m_isFailed(false),
    m_isWon(false),
    m_isInvincible(false),
    m_hasPlayedWinSound(false),
    m_hasPlayedDeathSound(false),
    m_lives(3),
    m_currentCharacter(Character::Mario),
    m_currentForm(PlayerForm::Small),
    m_animCounter(0),
    m_currentFrameIndex(0)
{
    m_type = ObjectType::Player;
    initPlayer();
    m_currentAnimations=&m_marioAnimations;
    setPixmap( (*m_currentAnimations)[m_currentForm].idle );
    setFlag(QGraphicsItem::ItemIsFocusable);
    setZValue(10);
}

// ==========================================
// ★ 核心切图引擎 (适配 9 列排版)
// ==========================================
SpriteSet Player::cutSpriteSheet(const QPixmap& sheet, int row, int frameWidth, int frameHeight, bool isSmall) {
    SpriteSet set;
    int y = row * frameHeight;

    set.idle = sheet.copy(0 * frameWidth, y, frameWidth, frameHeight);
    set.walkFrames.push_back(sheet.copy(1 * frameWidth, y, frameWidth, frameHeight));
    set.walkFrames.push_back(sheet.copy(2 * frameWidth, y, frameWidth, frameHeight));
    set.walkFrames.push_back(sheet.copy(3 * frameWidth, y, frameWidth, frameHeight));
    set.jump = sheet.copy(4 * frameWidth, y, frameWidth, frameHeight);

    if (isSmall) {
        set.skid = QPixmap(); 
        set.slideFrames.push_back(sheet.copy(5 * frameWidth, y, frameWidth, frameHeight));
        set.slideFrames.push_back(sheet.copy(6 * frameWidth, y, frameWidth, frameHeight));
        set.squat = sheet.copy(7 * frameWidth, y, frameWidth, frameHeight);
    } else {
        set.skid = sheet.copy(5 * frameWidth, y, frameWidth, frameHeight);
        set.slideFrames.push_back(sheet.copy(6 * frameWidth, y, frameWidth, frameHeight));
        set.slideFrames.push_back(sheet.copy(7 * frameWidth, y, frameWidth, frameHeight));
        set.squat = sheet.copy(8 * frameWidth, y, frameWidth, frameHeight);
    }
    return set;
}

// ==========================================
// ★ 资源加载与初始化 (精准对齐切图)
// ==========================================
void Player::initPlayer() {
    QPixmap normalSheet(":/image/normal.png");
    QPixmap greenSheet(":/image/green.png");
    QPixmap fireSheet(":/image/fire.png");

    // 🛡️ 保底机制：如果加载失败，变成纯色方块防崩溃
    if (normalSheet.isNull()) { normalSheet = QPixmap(144, 64); normalSheet.fill(Qt::red); }
    if (greenSheet.isNull()) { greenSheet = QPixmap(144, 64); greenSheet.fill(Qt::green); }
    if (fireSheet.isNull()) { fireSheet = QPixmap(144, 32); fireSheet.fill(Qt::magenta); }

    // 🌟 核心修复 1：【安全瘦身】
    // 强制把2行的图片高度缩放到 64，1行的缩放到 32（保持物理引擎稳定，防止暴毙）
    normalSheet = normalSheet.scaledToHeight(64, Qt::SmoothTransformation);
    greenSheet  = greenSheet.scaledToHeight(64, Qt::SmoothTransformation);
    fireSheet   = fireSheet.scaledToHeight(32, Qt::SmoothTransformation);

    // 🌟 核心修复 2：【强迫症级精准对齐】
    // 算出当前的宽度，然后微调一下，强行让它能被 9 完美整除，彻底消灭“各出现一半”的残影
    int mw = (normalSheet.width() / 9) * 9;
    normalSheet = normalSheet.scaled(mw, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    
    int gw = (greenSheet.width() / 9) * 9;
    greenSheet = greenSheet.scaled(gw, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    
    int fw = (fireSheet.width() / 9) * 9;
    fireSheet = fireSheet.scaled(fw, 32, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // 开始切图装载 (现在的高度严格是 32，宽度精确切分)
    m_marioAnimations[PlayerForm::Big]   = cutSpriteSheet(normalSheet, 0, mw/9, 32, false);
    m_marioAnimations[PlayerForm::Small] = cutSpriteSheet(normalSheet, 1, mw/9, 32, true);

    m_luigiAnimations[PlayerForm::Big]   = cutSpriteSheet(greenSheet, 0, gw/9, 32, false);
    m_luigiAnimations[PlayerForm::Small] = cutSpriteSheet(greenSheet, 1, gw/9, 32, true);

    m_marioAnimations[PlayerForm::Fire]  = cutSpriteSheet(fireSheet, 0, fw/9, 32, false);
    m_luigiAnimations[PlayerForm::Fire]  = cutSpriteSheet(fireSheet, 0, fw/9, 32, false);

    // 录入物理手感参数
    m_profiles[Character::Mario] = { 1.5, 1.5, 0, -5.6, 0.25 };
    m_profiles[Character::Luigi] = { 2.0, 0.10, 0.92, -5.8, 0.22 };
}

// ==========================================
// 控制与事件响应
// ==========================================
void Player::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        m_isLeftPressed = true;
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        m_isRightPressed = true;
    }
    else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_W) {
        if (m_isOnGround) {
            SoundManager::instance().playSound("jump");
            m_isJumpPressed = true;
            m_isOnGround = false;
        }
    }
    else if(event->key() == Qt::Key_S){
        if(m_currentForm != PlayerForm::Small)
            m_isSquatPressed = true;
        
        if((this->x() >= 715 && this->x() <= 740)){
            QTimer::singleShot(500, this, [=](){
                SoundManager::instance().playSound("pipe");
                m_isUnder = true;
                setPos(3785, 177);
            });
        }
    }
    else if(event->key() == Qt::Key_C){
        if(m_currentCharacter == Character::Mario) {
            m_currentCharacter = Character::Luigi;
            m_currentAnimations = &m_luigiAnimations;
        } else {
            m_currentCharacter = Character::Mario;
            m_currentAnimations = &m_marioAnimations;
        }
    }
    else if(event->key() == Qt::Key_K && m_currentForm == PlayerForm::Fire){
        if (event->isAutoRepeat()) return;
        
        FireBall* fireball = new FireBall(m_facingRight);
        emit scoreChanged(m_score);
        qreal spawnX = m_facingRight ? (x() + 16) : (x() - 16);
        qreal spawnY = y() + 8;
        fireball->setPos(spawnX, spawnY);
        this->scene()->addItem(fireball);
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) {
        m_isLeftPressed = false;
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) {
        m_isRightPressed = false;
    }
    else if(event->key() == Qt::Key_S){
        m_isSquatPressed = false;
    }
}

// ==========================================
// 核心游戏逻辑更新
// ==========================================
void Player::updateLogic()
{
    const PhysicsProfile& stats = m_profiles[m_currentCharacter];

    if (m_isWon) {
        qreal groundY = (m_currentForm == PlayerForm::Small) ? 160 : 144;
        SpriteSet currentPosture = (*m_currentAnimations)[m_currentForm];
        
        if (y() < groundY) {
            setPos(x(), y() + 1);
            static int frameCounter = 0;
            frameCounter++;
            if ((frameCounter / 8) % 2 == 0) {
                setPixmap(currentPosture.slideFrames[0]);
            } else {
                setPixmap(currentPosture.slideFrames[1]);
            }
        } else {
            if(x() < 3296) {
                if (!m_hasPlayedWinSound) {
                    SoundManager::instance().stopBGM();
                    SoundManager::instance().playSound("win");
                    m_hasPlayedWinSound = true;
                }
                m_vx = 2;
                m_vy = 0;
                m_isOnGround = true;
                setPos(x() + m_vx, groundY + 17);
                updateAnimation();
            } else {
                this->hide();
                emit gameWon();
            }
        }
        return;
    }

    if (m_isInvincible) {
        static int blinkCounter = 0;
        blinkCounter++;
        if ((blinkCounter / 5) % 2 == 0) {
            setOpacity(0.3);
        } else {
            setOpacity(1.0);
        }
    }

    if (m_isLeftPressed && !m_isRightPressed) {
        m_vx -= stats.acceleration;
        if (m_vx < -stats.maxSpeed) m_vx = -stats.maxSpeed;
    } else if (m_isRightPressed && !m_isLeftPressed) {
        m_vx += stats.acceleration;
        if (m_vx > stats.maxSpeed) m_vx = stats.maxSpeed;
    } else {
        m_vx *= stats.friction;
        if (std::abs(m_vx) < 0.1) m_vx = 0;
    }
    
    setPos(x() + m_vx, y());
    resolveTerrainCollisions(Qt::Horizontal);

    if (m_isJumpPressed && m_isOnGround) {
        m_vy = stats.jumpForce;
        m_isOnGround = false;
        m_isJumpPressed = false;
    }
    if(!m_isOnGround){
        m_vy += stats.gravity;
        if(m_vy > max_vy) m_vy = max_vy;
    }
    setPos(x(), y() + m_vy);

    if(this->y() > 224){
        this->m_lives--;
        emit lifeChanged(m_lives);
        if(m_lives > 0){
            m_isInvincible = true;
            QTimer::singleShot(2000, [this]() {
                m_isInvincible = false;
                this->setOpacity(1.0);
            });
            if (!m_hasPlayedDeathSound) {
                m_hasPlayedDeathSound = true;
                SoundManager::instance().playSound("death");
                m_hasPlayedDeathSound = false;
                setPos(50, 177);
                m_currentForm = PlayerForm::Small;
                m_currentCharacter = Character::Mario;
                m_currentAnimations = &m_marioAnimations;
            }
        }
    }
    
    m_isOnGround = false;
    resolveTerrainCollisions(Qt::Vertical);
    updateAnimation();
}

// ==========================================
// 碰撞与地形处理
// ==========================================
void Player::resolveTerrainCollisions(Qt::Orientation orientation)
{
    bool isMovingRight = (m_vx > 0);
    bool isMovingLeft  = (m_vx < 0);
    bool isFalling     = (m_vy >= 0);
    bool isJumping     = (m_vy < 0);
    
    QList<QGraphicsItem *> list = collidingItems();
    for (int i=0; i<list.size(); ++i) {
        GameObject *obj = dynamic_cast<GameObject*>(list[i]);
        if (!obj) continue;
        
        if (obj->getObjectType() == ObjectType::Brick || obj->getObjectType() == ObjectType::Pipe ) {
            QRectF myRect = this->sceneBoundingRect();
            QRectF objRect = obj->sceneBoundingRect();
            
            if (orientation == Qt::Horizontal) {
                if (myRect.bottom() <= objRect.top() + 2.0) continue;
                if (myRect.top() >= objRect.bottom() - 2.0) continue;
                if (isMovingRight) {
                    setPos(objRect.left() - myRect.width(), y());
                    m_vx = 0;
                } else if (isMovingLeft) {
                    setPos(objRect.right(), y());
                    m_vx = 0;
                }
            }
            else if (orientation == Qt::Vertical) {
                if (myRect.right() <= objRect.left() + 1.0) continue;
                if (myRect.left() >= objRect.right() - 1.0) continue;
                if (isFalling) {
                    setPos(x(), objRect.top() - myRect.height() + 1);
                    m_isOnGround = true;
                    m_vy = 0;
                } else if (isJumping) {
                    setPos(x(), objRect.bottom());
                    m_vy = 0;
                    obj->onCollision(this);
                    emit scoreChanged(m_score);
                    emit coinChanged(m_coin);
                }
            }
        }
        else if (obj->getObjectType() == ObjectType::Mushroom || obj->getObjectType() == ObjectType::Enemy
                   || obj->getObjectType() == ObjectType::Flower || obj->getObjectType() == ObjectType::FlagPole
                   || obj->getObjectType() == ObjectType::Piranha || obj->getObjectType() == ObjectType::Coin) {
            this->onCollision(obj);
            obj->onCollision(this);
        }
    }
}

void Player::onCollision(GameObject* other)
{
    if(other->getObjectType() == ObjectType::Enemy){
        Enemy* enemy = dynamic_cast<Enemy*>(other);
        if (!enemy || enemy->isDead()) return;
        
        if(this->m_vy > 0 && (this->y() + this->boundingRect().height() / 2.0 < enemy->y() + enemy->boundingRect().height() / 2.0)){
            this->m_vy = -1;
            this->m_score += 100;
            emit scoreChanged(m_score);
            enemy->die();
        }
        else{
            if (m_isInvincible) return;
            if(getPlayerForm() == PlayerForm::Big || getPlayerForm() == PlayerForm::Fire){
                changeForm(PlayerForm::Small);
                m_isInvincible = true;
                QTimer::singleShot(2000, [this]() {
                    m_isInvincible = false;
                    this->setOpacity(1.0);
                });
            }
            else {
                m_lives--;
                emit lifeChanged(m_lives);
                if(m_lives <= 0)
                    m_isFailed = true;
                else{
                    m_isInvincible = true;
                    QTimer::singleShot(2000, [this]() {
                        m_isInvincible = false;
                        this->setOpacity(1.0);
                    });
                    if (!m_hasPlayedDeathSound) {
                        m_hasPlayedDeathSound = true;
                        SoundManager::instance().playSound("death");
                        m_hasPlayedDeathSound = false;
                    }
                }
            }
        }
    }
    else if(other->getObjectType() == ObjectType::Piranha){
        PiranhaPlant* piranha = dynamic_cast<PiranhaPlant*>(other);
        if (!piranha || piranha->isDead()) return;
        if (m_isInvincible) return;
        
        if(getPlayerForm() == PlayerForm::Big || getPlayerForm() == PlayerForm::Fire){
            changeForm(PlayerForm::Small);
            m_isInvincible = true;
            QTimer::singleShot(2000, [this]() {
                m_isInvincible = false;
                this->setOpacity(1.0);
            });
        }
        else {
            m_lives--;
            emit lifeChanged(m_lives);
            if(m_lives <= 0)
                m_isFailed = true;
            else{
                m_isInvincible = true;
                QTimer::singleShot(2000, [this]() {
                    m_isInvincible = false;
                    this->setOpacity(1.0);
                });
                if (!m_hasPlayedDeathSound) {
                    m_hasPlayedDeathSound = true;
                    SoundManager::instance().playSound("death");
                    m_hasPlayedDeathSound = false;
                }
            }
        }
    }
    else if(other->getObjectType() == ObjectType::Coin){
        StaticCoin* coin = dynamic_cast<StaticCoin*>(other);
        if (coin && !coin->m_isCollected) {
            coin->m_isCollected = true;
            SoundManager::instance().playSound("coin");
            m_coin++;
            emit coinChanged(m_coin);
            m_score += 200;
            emit scoreChanged(m_score);
            coin->hide();
            coin->deleteLater();
        }
    }
    else if(other->getObjectType() == ObjectType::Mushroom){
        if(m_currentForm == PlayerForm::Small){
            changeForm(PlayerForm::Big);
        }
        SoundManager::instance().playSound("powerUp");
        addScore(1000);
        emit scoreChanged(m_score);
    }
    else if(other->getObjectType() == ObjectType::Flower){
        if(m_currentForm == PlayerForm::Small || m_currentForm == PlayerForm::Big){
            changeForm(PlayerForm::Fire);
        }
        SoundManager::instance().playSound("powerUp");
        addScore(1000);
        emit scoreChanged(m_score);
    }
    else if (other->getObjectType() == ObjectType::FlagPole && !m_isWon) {
        m_isWon = true;
        m_isLeftPressed = false;
        m_isRightPressed = false;
        m_isJumpPressed = false;
        setPos(other->x() - 4, y());
        m_vx = 0;
        m_vy = 0;
        SoundManager::instance().playSound("flagpole");
        QList<QGraphicsItem*> items = this->scene()->items();
        for (int i=0; i<items.size(); ++i) {
            Flag* flag = dynamic_cast<Flag*>(items[i]);
            if (flag) {
                flag->m_isSliding = true;
                break;
            }
        }
    }
}

// ==========================================
// 动画与形态改变
// ==========================================
void Player::updateAnimation()
{
    QPixmap currentImage;
    SpriteSet currentPosture = (*m_currentAnimations)[m_currentForm];
    
    bool isSkidding = m_isOnGround &&
                      ((m_isLeftPressed && m_vx > 0.5) ||
                       (m_isRightPressed && m_vx < -0.5));

    if (isSkidding && !currentPosture.skid.isNull()) {
        currentImage = currentPosture.skid;
    }
    else if (!m_isOnGround) {
        currentImage = currentPosture.jump;
    }
    else if (m_vx != 0) {
        m_animCounter++;
        if (m_animCounter >= 5) {
            m_animCounter = 0;
            m_currentFrameIndex = (m_currentFrameIndex + 1) % currentPosture.walkFrames.size();
        }
        if (!currentPosture.walkFrames.isEmpty()) {
            currentImage = currentPosture.walkFrames[m_currentFrameIndex];
        } else {
            currentImage = currentPosture.idle;
        }
    }
    else if(m_isOnGround && m_vx == 0 && m_isSquatPressed && !currentPosture.squat.isNull()){
        currentImage = currentPosture.squat;
        m_currentFrameIndex = 0;
    }
    else if(m_isOnGround && m_vx == 0){
        currentImage = currentPosture.idle;
        m_currentFrameIndex = 0;
    }

    if (m_vx > 0 ) m_facingRight = true;
    else if (m_vx < 0) m_facingRight = false;

    if (!m_facingRight && !currentImage.isNull()) {
        currentImage = currentImage.transformed(QTransform().scale(-1, 1));
    }
    
    if (!currentImage.isNull()) {
        setPixmap(currentImage);
    }
}

void Player::changeForm(PlayerForm newForm){
    if (m_currentForm == newForm) {
        return;
    }

    int oldHeight = this->boundingRect().height();
    m_currentForm = newForm;
    setPixmap( (*m_currentAnimations)[m_currentForm].idle );
    int newHeight = this->boundingRect().height();
    
    if (oldHeight != newHeight) {
        setPos(x(), y() + (oldHeight - newHeight));
    }
}