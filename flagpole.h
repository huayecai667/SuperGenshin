#ifndef FLAGPOLE_H
#define FLAGPOLE_H
#include"staticobject.h"
#include <QPixmap>
#include <QPainter>
class FlagPole:public StaticObject
{
public:
    explicit FlagPole(QGraphicsItem *parent = nullptr);
    virtual ~FlagPole()=default;

};

#endif // FLAGPOLE_H
