#include "gameobject.h"

GameObject::GameObject(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), m_type(ObjectType::Unknown)
{
    // 初始默认类型为 Unknown
}
