#ifndef FIREBALL_H
#define FIREBALL_H
#include "dynamicobject.h"
class FireBall:public QObject,public DynamicObject
{
    Q_OBJECT
public:
    explicit FireBall(bool isFacingRight,QGraphicsItem* parent=nullptr);
    virtual ~FireBall()=default;

    virtual void updateLogic() override;
    virtual void onCollision(GameObject* other) override;
private:
    bool resolveTerrainCollisions(Qt::Orientation orientation);
    void boom();
};

#endif // FIREBALL_H
