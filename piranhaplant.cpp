// piranha_plant.cpp
#include "piranhaplant.h"
#include "player.h"
#include <QGraphicsScene>
#include <QTimer>
PiranhaPlant::PiranhaPlant(qreal pipeTopCenterX, qreal pipeTopY, QGraphicsItem* parent)
    : DynamicObject(parent), m_state(PlantState::MovingUp), m_waitTimer(0), m_animCounter(0)
{
    m_type = ObjectType::Piranha;
    setZValue(4);
    setPixmap(QPixmap(":/image/piranha_0.png"));
    qreal startX = pipeTopCenterX - 8;
    m_hiddenY = pipeTopY + 2;
    m_exposedY = pipeTopY - 24;
    setPos(startX, m_hiddenY);
}

void PiranhaPlant::updateLogic() {
    if (m_isDead) return;
    m_animCounter++;
    if ((m_animCounter / 10) % 2 == 0) {
        setPixmap(QPixmap(":/image/piranha_0.png"));
    } else {
        setPixmap(QPixmap(":/image/piranha_1.png"));
    }
    qreal currentY = y();
    qreal speed = 1.0;

    switch (m_state) {
    case PlantState::MovingUp:
        currentY -= speed;
        if (currentY <= m_exposedY) {
            currentY = m_exposedY;
            m_state = PlantState::StayingUp;
            m_waitTimer = 0;
        }
        break;

    case PlantState::StayingUp:
        m_waitTimer++;
        if (m_waitTimer >= 100) {
            m_state = PlantState::MovingDown;
        }
        break;

    case PlantState::MovingDown:
        currentY += speed;
        if (currentY >= m_hiddenY) {
            currentY = m_hiddenY;
            m_state = PlantState::StayingDown;
            m_waitTimer = 0;
        }
        break;

    case PlantState::StayingDown:
        m_waitTimer++;
        if (m_waitTimer >= 150) {
            m_state = PlantState::MovingUp;
        }
        break;
    }
    setPos(x(), currentY);

    if (m_state != PlantState::StayingDown && !m_isDead) {
        QList<QGraphicsItem*> items = collidingItems();
        for (auto* item : items) {
            GameObject *obj = dynamic_cast<GameObject*>(item);

            if (!obj) continue;

            if(obj->getObjectType() == ObjectType::Fireball){
                m_isDead = true;

                QTimer::singleShot(300, [this]() {
                    if (this->scene()) {
                        this->scene()->removeItem(this);
                    }
                    delete this;
                });

                break;
            }
        }
    }
}
void PiranhaPlant::die() {
    if (m_isDead) return;

    m_isDead = true;
    Player::addScore(100);
    QTimer::singleShot(300, [this]() {
        if (this->scene()) {
            this->scene()->removeItem(this);
        }
        delete this;
    });
}
