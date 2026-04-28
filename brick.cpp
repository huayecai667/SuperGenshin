#include "brick.h"
#include "player.h"
#include "mushroom.h"
#include "flower.h"
#include "dynamiccoin.h"
#include "soundmanager.h"
#include<QTimer>
#include<QGraphicsScene>
Brick::Brick(BrickType type, int unkown, QGraphicsItem *parent)
    : QObject(),
    StaticObject(parent),
    m_brickType(type),
    m_currentFrameIndex(0),
    m_animTimer(nullptr)
{
    m_type = ObjectType::Brick;

    switch (m_brickType) {
    case BrickType::Ground:
        setPixmap(QPixmap(":/image/ground.png"));
        break;

    case BrickType::BreakableBrick:
        setPixmap(QPixmap(":/image/brick.png"));
        break;

    case BrickType::UnbreakableBrick:
        setPixmap(QPixmap(":/image/stone.png"));
        break;

    case BrickType::UsedQuestionBlock:
        setPixmap(QPixmap(":/image/question_used.png"));
        break;

    case BrickType::QuestionBlock:
        m_animFrames.push_back(QPixmap(":/image/question_0.png"));
        m_animFrames.push_back(QPixmap(":/image/question_1.png"));
        m_animFrames.push_back(QPixmap(":/image/question_2.png"));
        m_animFrames.push_back(QPixmap(":/image/question_3.png"));
        unkowntype=unkown;
        setPixmap(m_animFrames[0]);

        //启动动画定时器
        m_animTimer = new QTimer(this);
        connect(m_animTimer, &QTimer::timeout, this, &Brick::updateAnimation);
        m_animTimer->start(200);
        break;

    case BrickType::UnderGround:
        setPixmap(QPixmap(":/image/underground.png"));
        break;

    case BrickType::UnderBrick:
        setPixmap(QPixmap(":/image/underbrick.png"));
        break;
    }
    setZValue(10);
}

void Brick::onCollision(GameObject* other)
{
    if (other->getObjectType() == ObjectType::Player) {
        Player* player=dynamic_cast<Player*>(other);
        PlayerForm currentForm=player->getPlayerForm();

        if (m_brickType == BrickType::QuestionBlock) {
            transformToUsed();
        }
        if(m_brickType==BrickType::BreakableBrick && currentForm!=PlayerForm::Small){
            brickBreak();
        }

    }
}
void Brick::updateAnimation()
{
    if (m_animFrames.isEmpty()) return;
    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animFrames.size();
    setPixmap(m_animFrames[m_currentFrameIndex]);
}

void Brick::transformToUsed(){
        //停止并清理动画计时器
        if (m_animTimer) {
            m_animTimer->stop();
            m_animTimer->deleteLater();
            m_animTimer = nullptr;
        }
        m_brickType = BrickType::UsedQuestionBlock;
        setPixmap(QPixmap(":/image/question_used.png"));
        if(unkowntype==1){
            Mushroom* mushroom=new Mushroom();
            mushroom->setPos(this->x(),this->y()-16);
            this->scene()->addItem(mushroom);
            SoundManager::instance().playSound("powerAppear");

        }
        else if(unkowntype==2){
            Flower* flower=new Flower();
            flower->setPos(this->x(),this->y()-16);
            this->scene()->addItem(flower);
            SoundManager::instance().playSound("powerAppear");
        }
        else
        {
            DynamicCoin* coin=new DynamicCoin();
            coin->setPos(this->x(),this->y()-16);
            this->scene()->addItem(coin);
            Player::addScore(200);
            Player::addCoin();
            SoundManager::instance().playSound("coin");
        }

}

void Brick::brickBreak(){
    QGraphicsScene* currentScene = this->scene();
    if (!currentScene) return;
    SoundManager::instance().playSound("smash");
    qreal bx=x()+8;
    qreal by=y()+8;
    this->hide();
    this->setPos(-1000, -1000);
    QPixmap breakImg0(QPixmap(":/image/breakbrick_0"));
    QPixmap breakImg1(breakImg0.transformed(QTransform().scale(-1, 1)));
    QPixmap breakImg2(QPixmap(":/image/breakbrick_1"));
    QPixmap breakImg3(breakImg2.transformed(QTransform().scale(-1, 1)));
    QGraphicsPixmapItem* p0 = new QGraphicsPixmapItem(breakImg0);
    QGraphicsPixmapItem* p1 = new QGraphicsPixmapItem(breakImg1);
    QGraphicsPixmapItem* p2 = new QGraphicsPixmapItem(breakImg2);
    QGraphicsPixmapItem* p3 = new QGraphicsPixmapItem(breakImg3);
    p0->setPos(bx-16, by-16);
    p1->setPos(bx, by-16);
    p2->setPos(bx-16, by);
    p3->setPos(bx, by);
    currentScene->addItem(p0);
    currentScene->addItem(p1);
    currentScene->addItem(p2);
    currentScene->addItem(p3);
    struct DebrisData {
        QGraphicsPixmapItem* item;
        double vx, vy;
    };
    QList<DebrisData> debrisList = {
        {p0, -2.0, -2.0},
        {p1,  2.0, -2.0},
        {p2, -2.0, -1.0},
        {p3,  2.0, -1.0}
    };
    QTimer* animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, [=]() mutable {
        for (auto& d : debrisList) {
            d.vy+=0.4;
            d.item->setPos(d.item->x() + d.vx, d.item->y() + d.vy);
        }
    });
    animTimer->start(16);
    QTimer::singleShot(2000, this, [=]() {
        animTimer->stop();
        for (auto& d : debrisList) {
            delete d.item;
        }
        this->deleteLater();
    });
}
