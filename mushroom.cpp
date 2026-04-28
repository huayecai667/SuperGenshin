#include "mushroom.h"

Mushroom::Mushroom(QGraphicsItem* parent)
    :QObject(),
    DynamicObject(parent)
{
    m_type=ObjectType::Mushroom;
    setZValue(9);
    setPixmap(QPixmap(":/image/mushroom.png"));
    m_vx=0.4;
}

void Mushroom::updateLogic(){
    setPos(x() + m_vx, y());
    resolveTerrainCollisions(Qt::Horizontal);
    m_vy = std::clamp(m_vy + 0.5, 0.0, 10.0);
    setPos(x(), y() + m_vy);
    resolveTerrainCollisions(Qt::Vertical);
}

void Mushroom::resolveTerrainCollisions(Qt::Orientation orientation){
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
                if (myRect.right() <= objRect.left() + 2.0) continue;
                if (myRect.left() >= objRect.right() - 2.0) continue;

                if (m_vy > 0) {
                    setPos(x(), objRect.top() - myRect.height() + 0.1);
                    m_isOnGround = true;
                    m_vy = 0;
                }
            }
        }
    }
}
void Mushroom::onCollision(GameObject* other){
    if (other->getObjectType() == ObjectType::Player)
        this->deleteLater();
}
