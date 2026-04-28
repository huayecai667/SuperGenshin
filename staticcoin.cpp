#include "staticcoin.h"
#include "qgraphicsitem.h"

StaticCoin::StaticCoin(QGraphicsItem *parent)
    :QObject(),
    StaticObject(parent),
    m_currentFrameIndex(0),
    m_animTimer(nullptr)
{
    m_type=ObjectType::Coin;
    m_animFrames.push_back(QPixmap(":/image/coin_0.png"));
    m_animFrames.push_back(QPixmap(":/image/coin_1.png"));
    m_animFrames.push_back(QPixmap(":/image/coin_2.png"));
    m_animFrames.push_back(QPixmap(":/image/coin_3.png"));
    setPixmap(m_animFrames[0]);
    m_animTimer = new QTimer(this);
    connect(m_animTimer, &QTimer::timeout, this, &StaticCoin::updateAnimation);
    m_animTimer->start(200);
    setZValue(9);
}
void StaticCoin::updateAnimation()
{
    if (m_animFrames.isEmpty()) return;
    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animFrames.size();
    setPixmap(m_animFrames[m_currentFrameIndex]);
}

void StaticCoin::onCollision(GameObject* other)
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
