#ifndef CASTLE_H
#define CASTLE_H
#include"staticobject.h"
#include <QPixmap>
class Castle:public StaticObject
{
public:
    explicit Castle(QGraphicsItem *parent = nullptr);
    virtual ~Castle() = default;
};

#endif // CASTLE_H
