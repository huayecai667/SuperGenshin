#include "pipe.h"

Pipe::Pipe(int height,QGraphicsItem *parent)
    : StaticObject(parent)
{
    m_type = ObjectType::Pipe;
    //32*16
    QPixmap pipe(32,16*height);
    pipe.fill(Qt::transparent);
    QPainter painter(&pipe);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false); // 像素风格关闭抗锯齿
    QPixmap pipeTop(":/image/pipe_1.png");
    QPixmap pipeMid(":/image/pipe_0.png");
    painter.drawPixmap(0, 0, pipeTop);
    for(int i=1;i<height;++i){
        painter.drawPixmap(0, 16*i, pipeMid);
    }
    painter.end();
    setPixmap(pipe);
    setZValue(5);
}
Pipe::Pipe(QGraphicsItem *parent)
    : StaticObject(parent)
{
    m_type = ObjectType::Pipe;
    //48*32
    setZValue(6);
    setPixmap(QPixmap(":/image/pipe_2.png"));
}
