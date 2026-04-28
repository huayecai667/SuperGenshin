#ifndef DYNAMICCOIN_H
#define DYNAMICCOIN_H
#include "dynamicobject.h"
class DynamicCoin:public QObject,public DynamicObject
{
    Q_OBJECT
public:
    explicit DynamicCoin(QGraphicsItem* parent=nullptr);
    virtual ~DynamicCoin()=default;
    virtual void updateLogic() override;
private slots:
    void updateAnimation();
private:
    QVector<QPixmap> m_animFrames;
    int m_animDelay = 0;
    int m_currentFrameIndex=0;
    QTimer* m_animTimer;
};

#endif // DYNAMICCOIN_H
