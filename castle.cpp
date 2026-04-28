#include "castle.h"
Castle::Castle(QGraphicsItem *parent)
    : StaticObject(parent)
{
    m_type=ObjectType::Castle;
    setZValue(9);
    setPixmap(QPixmap(":/image/castle.png"));
}
