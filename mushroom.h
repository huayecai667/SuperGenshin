#ifndef MUSHROOM_H
#define MUSHROOM_H
#include "dynamicobject.h"
class Mushroom:public QObject,public DynamicObject
{
    Q_OBJECT
public:
    explicit Mushroom(QGraphicsItem* parent=nullptr);
    virtual ~Mushroom()=default;

    virtual void updateLogic() override;
    virtual void onCollision(GameObject* other) override;
private:
    void resolveTerrainCollisions(Qt::Orientation orientation);
};

#endif // MUSHROOM_H
