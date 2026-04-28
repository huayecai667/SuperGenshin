#ifndef PIPE_H
#define PIPE_H
#include"staticobject.h"
#include <QPixmap>
#include <QPainter>
class Pipe:public StaticObject
{
public:
    explicit Pipe(int height,QGraphicsItem *parent = nullptr);
    Pipe(QGraphicsItem *parent = nullptr);
    virtual ~Pipe()=default;

};

#endif // PIPE_H
