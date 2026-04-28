#include "dynamicobject.h"

DynamicObject::DynamicObject(QGraphicsItem *parent)
    : GameObject(parent),
    m_vx(0.0),
    m_vy(0.0),
    max_vy(12.0),
    m_isOnGround(false),
    m_facingRight(true)
{
}

void DynamicObject::updateLogic()
{
    setPos(x() + m_vx, y() + m_vy);
}
