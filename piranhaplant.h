#ifndef PIRANHAPLANT_H
#define PIRANHAPLANT_H

#include "dynamicobject.h"

class PiranhaPlant : public DynamicObject {
public:
    //传入水管顶部的中心坐标，方便定位
    explicit PiranhaPlant(qreal pipeTopCenterX, qreal pipeTopY, QGraphicsItem* parent = nullptr);
    virtual void updateLogic() override;
    void die();
    bool isDead() const { return m_isDead; }


private:
    enum class PlantState {
        MovingUp,
        StayingUp,
        MovingDown,
        StayingDown
    };
    bool m_isDead = false;
    PlantState m_state;
    qreal m_hiddenY;   //藏在水管里的y坐标
    qreal m_exposedY;  //完全钻出来的y坐标
    int m_waitTimer;   //停留时间计时器
    int m_animCounter;
};

#endif // PIRANHAPLANT_H#ifndef PIRANHAPLANT_H
#define PIRANHAPLANT_H

