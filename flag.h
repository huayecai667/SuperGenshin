#ifndef FLAG_H
#define FLAG_H
#include "dynamicobject.h"
class Flag:public QObject,public DynamicObject
{    Q_OBJECT
public:
    explicit Flag(QGraphicsItem* parent=nullptr);
    virtual ~Flag()=default;
    virtual void updateLogic() override;
    bool m_isSliding=false;
};

#endif // FLAG_H
