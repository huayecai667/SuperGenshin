#ifndef STATICOBJECT_H
#define STATICOBJECT_H
#include"gameobject.h"
class StaticObject:public GameObject
{
public:
    explicit StaticObject(QGraphicsItem *parent = nullptr);
    virtual ~StaticObject() = default;
    virtual void updateLogic() override {}
};

#endif // STATICOBJECT_H
