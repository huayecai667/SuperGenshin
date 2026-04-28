#include "fireball.h"
#include "piranhaplant.h"
#include<QGraphicsScene>
#include<QTimer>
FireBall::FireBall(bool isFacingRight,QGraphicsItem* parent)
    :QObject(),
    DynamicObject(parent)
{
    m_type=ObjectType::Fireball;
    setZValue(9);
    setPixmap(QPixmap(":/image/fireball.png"));
    if(isFacingRight)
        m_vx=3.0;
    else
        m_vx=-3.0;
    m_vy=2.0;

    QTimer::singleShot(4000, this, [this]() {
        this->boom();
    });

}
void FireBall::updateLogic(){
    setPos(x() + m_vx, y());
    if (resolveTerrainCollisions(Qt::Horizontal)) {
        return;
    }
    m_vy = std::clamp(m_vy + 0.5, -5.0, 5.0);
    setPos(x(), y() + m_vy);
    resolveTerrainCollisions(Qt::Vertical);
}
bool FireBall::resolveTerrainCollisions(Qt::Orientation orientation){
    QList<QGraphicsItem *> collidingList = collidingItems();
    bool isFalling = (m_vy > 0);
    bool isRising = (m_vy < 0);
    for (auto *item : collidingList) {
        GameObject *obj = dynamic_cast<GameObject*>(item);
        if (!obj) continue;
        if (obj->getObjectType() == ObjectType::Brick||obj->getObjectType() == ObjectType::Pipe) {

            QRectF myRect = this->sceneBoundingRect();
            QRectF objRect = obj->sceneBoundingRect();

            if (orientation == Qt::Horizontal) {
                if (myRect.bottom() <= objRect.top() + 4.0) continue;
                if (myRect.top() >= objRect.bottom() - 2.0) continue;
                boom();
                return true;
            }
            else if (orientation == Qt::Vertical) {
                if (myRect.right() <= objRect.left() + 2.0) continue;
                if (myRect.left() >= objRect.right() - 2.0) continue;

                if (isFalling) {
                    setPos(x(), objRect.top() - myRect.height());
                    m_vy = -5.0; // 固定反弹力
                }
                else if(isRising) {
                    if (myRect.top() < objRect.bottom() && myRect.bottom() > objRect.top() + 4.0) {
                        setPos(x(), objRect.bottom());
                        m_vy = 0;
                    }
                }
            }
        }
        else if(obj->getObjectType() == ObjectType::Piranha){
            PiranhaPlant* piranha = dynamic_cast<PiranhaPlant*>(obj);
            if (piranha) {

                piranha->die();
            }
            boom();
            return true;
        }

    }
    return false;
}

void FireBall::onCollision(GameObject* other){
    if (other->getObjectType() == ObjectType::Enemy)
        boom();
}

void FireBall::boom(){
    QGraphicsScene* currentScene = this->scene();
    if (!currentScene) return;
    qreal fx=x();
    qreal fy=y();
    this->hide();
    this->setPos(-1000,1000);
    QPixmap boompix(QPixmap(":/image/boom_0.png"));
    QGraphicsPixmapItem* b = new QGraphicsPixmapItem(boompix);
    b->setPos(fx-8,fy-8);
    currentScene->addItem(b);
    QTimer::singleShot(200, this, [=]() {
        b->setPixmap(QPixmap(":/image/boom_1.png"));
    });
    QTimer::singleShot(400, this, [=]() {
        delete b;
        this->deleteLater();
    });
}
