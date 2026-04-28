#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H
#include "gameobject.h"
class DynamicObject:public GameObject
{
public:
    explicit DynamicObject(QGraphicsItem *parent = nullptr);
    virtual ~DynamicObject() = default;
    // 重写帧更新逻辑，在这里统一处理所有动态物体x和y方向上的位移
    virtual void updateLogic() override;

protected:
    // 物理引擎核心变量
    double m_vx;
    double m_vy;         //Y轴速度 (正数向下掉落，负数向上跳跃)
    double max_vy;

    // 状态标志
    bool m_isOnGround;
    bool m_facingRight;  //方便翻转贴图
};

#endif // DYNAMICOBJECT_H
