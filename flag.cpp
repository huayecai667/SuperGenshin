#include "flag.h"

Flag::Flag(QGraphicsItem* parent)
    :QObject(),
    DynamicObject(parent)
{
    m_type=ObjectType::Flag;
    setZValue(9);
    setPixmap(QPixmap(":/image/flag.png"));
}
void Flag::updateLogic(){
    if (m_isSliding) {
        if (this->y() < 160) {
            setPos(x(), y()+1);
        } else {
            m_isSliding = false;
        }
    }
}
