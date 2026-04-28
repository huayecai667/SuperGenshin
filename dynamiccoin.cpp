#include "dynamiccoin.h"
#include <QTimer>
#include <algorithm>
DynamicCoin::DynamicCoin(QGraphicsItem* parent)
    : QObject(),
    DynamicObject(parent)
{
    m_type=ObjectType::Coin;
    setZValue(9);
    m_animFrames.push_back(QPixmap(":/image/coin_4.png"));
    m_animFrames.push_back(QPixmap(":/image/coin_5.png"));
    m_animFrames.push_back(QPixmap(":/image/coin_6.png"));
    m_animFrames.push_back(QPixmap(":/image/coin_7.png"));
    setPixmap(m_animFrames[0]);
    m_vy=-4;
    m_animTimer = new QTimer(this);
    connect(m_animTimer, &QTimer::timeout, this, &DynamicCoin::updateAnimation);
    m_animTimer->start(100);

}
void DynamicCoin::updateAnimation()
{
    if (m_animFrames.isEmpty()) return;
    m_currentFrameIndex = (m_currentFrameIndex + 1) % m_animFrames.size();
    setPixmap(m_animFrames[m_currentFrameIndex]);
}

void DynamicCoin::updateLogic(){
    m_vy = std::clamp(m_vy + 0.2, -4.0, 4.0);
    setPos(x(),y()+m_vy);
    if(m_vy==4.0){
        if (m_animTimer) {
            m_animTimer->stop();
            m_animTimer->deleteLater();
            m_animTimer = nullptr;
        }
        this->deleteLater();
    }
}
