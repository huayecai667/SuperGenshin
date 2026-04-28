#include "flower.h"

Flower::Flower(QGraphicsItem *parent)
    :QObject(),
    StaticObject(parent),
    m_currentFrameIndex(0),
    m_animTimer(nullptr)
{
    m_type=ObjectType::Flower;
    m_animFrames.push_back(QPixmap(":/image/flower_0.png"));
    m_animFrames.push_back(QPixmap(":/image/flower_1.png"));
    m_animFrames.push_back(QPixmap(":/image/flower_2.png"));
    m_animFrames.push_back(QPixmap(":/image/flower_3.png"));
    setPixmap(m_animFrames[0]);
    m_animTimer = new QTimer(this);
    connect(m_animTimer, &QTimer::timeout, this, &Flower::updateAnimation);
    m_animTimer->start(200);
    setZValue(9);
}

void Flower::updateAnimation()
{
    if (m_animFrames.isEmpty()) return;
    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animFrames.size();
    setPixmap(m_animFrames[m_currentFrameIndex]);
}

void Flower::onCollision(GameObject* other)
{
    if (other->getObjectType() == ObjectType::Player) {
        if (m_animTimer) {
            m_animTimer->stop();
            m_animTimer->deleteLater();
            m_animTimer = nullptr;
        }
        this->deleteLater();
    }
}
