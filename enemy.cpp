#include "enemy.h"
#include "player.h"
#include "soundmanager.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>
#include <algorithm>
#include<QDebug>
#include<QTimer>
Enemy::Enemy(QGraphicsItem* parent)
    : QObject(),
    DynamicObject(parent),
    m_isActive(false)
{
    m_type=ObjectType::Enemy;
    setZValue(9);
    m_animFrames.push_back(QPixmap(":/image/goomba_0.png"));
    m_animFrames.push_back(QPixmap(":/image/goomba_1.png"));
    setPixmap(m_animFrames.first());
    m_vx=-0.3;
}
void Enemy::updateLogic(){
    if (m_isDead) {
        return;
    }
    if (!m_playerTarget) {
        for (QGraphicsItem* item : scene()->items()) {
            GameObject* obj = dynamic_cast<GameObject*>(item);
            if (obj && obj->getObjectType() == ObjectType::Player) {
                m_playerTarget = obj;
                break;
            }
        }
        if (!m_playerTarget) return;
    }

    double distanceX = this->x() - m_playerTarget->x();

    if (!m_isActive) {
        if (distanceX <= 324.0) {
            m_isActive = true;
        } else {
            return;
        }
    }
    if (distanceX < -324.0 || this->y() > 244) {
        this->deleteLater();
        return;
    }

    setPos(x() + m_vx, y());
    resolveTerrainCollisions(Qt::Horizontal);
    m_vy = std::clamp(m_vy + 0.5, 0.0, 10.0);
    setPos(x(), y() + m_vy);
    resolveTerrainCollisions(Qt::Vertical);

    m_animDelay++;
    if (m_animDelay >= 15) {
        m_animDelay = 0;
        m_currentFrameIndex++;
        if (m_currentFrameIndex >= m_animFrames.size()) {
            m_currentFrameIndex = 0;
        }
    }

    QPixmap currentImage = m_animFrames[m_currentFrameIndex];

    m_facingRight = (m_vx > 0);
    if (!m_facingRight) {
        currentImage = currentImage.transformed(QTransform().scale(-1, 1));
    }
    setPixmap(currentImage);
}

void Enemy::resolveTerrainCollisions(Qt::Orientation orientation){
    QList<QGraphicsItem *> collidingList = collidingItems();
    for (auto *item : collidingList) {
        GameObject *obj = dynamic_cast<GameObject*>(item);
        if (!obj) continue;
        if (obj->getObjectType() == ObjectType::Brick||obj->getObjectType() == ObjectType::Pipe) {

            QRectF myRect = this->sceneBoundingRect();
            QRectF objRect = obj->sceneBoundingRect();

            if (orientation == Qt::Horizontal) {
                if (myRect.bottom() <= objRect.top() + 13.0) continue;
                if (myRect.top() >= objRect.bottom() - 13.0) continue;

                if (m_vx > 0) {
                    setPos(objRect.left() - myRect.width(), y());
                    m_vx = -m_vx;
                } else if (m_vx < 0) {
                    setPos(objRect.right(), y());
                    m_vx = -m_vx;
                }
            }
            else if (orientation == Qt::Vertical) {
                if (myRect.right() <= objRect.left() + 1.0) continue;
                if (myRect.left() >= objRect.right() - 1.0) continue;

                if (m_vy > 0) {
                    setPos(x(), objRect.top() - myRect.height() + 0.1);
                    m_isOnGround = true;
                    m_vy = 0;
                }
            }
        }
        else if(obj->getObjectType()==ObjectType::Fireball){
            this->onCollision(obj);
            obj->onCollision(this);
        }
    }
}

void Enemy::onCollision(GameObject* other)
{
    if(other->getObjectType()==ObjectType::Fireball){
        die();
        Player::addScore(100);
    }
}

void Enemy::die(){
    if (m_isDead)
        return;
    m_isDead = true;
    this->m_vx=0;
    SoundManager::instance().playSound("stomp");
    setPixmap(QPixmap(":/image/goomba_die.png"));
    QTimer::singleShot(300, this, [this](){
        this->deleteLater();
    });
}
