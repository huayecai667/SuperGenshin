#include "flagpole.h"

FlagPole::FlagPole(QGraphicsItem *parent)
    :StaticObject(parent)
{
    m_type = ObjectType::FlagPole;
    QPixmap flagPole(8,8+16*9);
    flagPole.fill(Qt::transparent);
    QPainter painter(&flagPole);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false); // 像素风格关闭抗锯齿
    QPixmap poleTop(":/image/flagpoleball.png");
    QPixmap poleMid(":/image/flagpole.png");
    painter.drawPixmap(0, 0, poleTop);
    for(int i=0;i<9;++i){
        painter.drawPixmap(0, 8+16*i, poleMid);
    }
    painter.end();
    setPixmap(flagPole);
    setZValue(5);
}
