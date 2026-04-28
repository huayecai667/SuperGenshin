#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<QGraphicsPixmapItem>

enum class ObjectType {
    Player,
    Enemy,
    Piranha,
    Brick,
    Fireball,
    Coin,
    Mushroom,
    Flower,
    Pipe,
    Castle,
    Flag,
    FlagPole,
    Unknown
};

class GameObject:public QGraphicsPixmapItem
{
public:
    GameObject(QGraphicsItem *parent = nullptr);
    virtual ~GameObject() = default;
    // 获取物体类型的方法，供外部碰撞检测时快速判断
    ObjectType getObjectType() const { return m_type; }
    //帧更新
    virtual void updateLogic() {}
    //碰撞响应
    virtual void onCollision(GameObject* other) {}
protected:
    ObjectType m_type;
};

#endif // GAMEOBJECT_H
