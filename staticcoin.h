#ifndef STATICCOIN_H
#define STATICCOIN_H
#include"staticobject.h"
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QObject>
class StaticCoin:public QObject,public StaticObject
{
    Q_OBJECT
public:
    explicit StaticCoin(QGraphicsItem *parent = nullptr);
    virtual ~StaticCoin() = default;
    virtual void onCollision(GameObject* other) override;
    bool m_isCollected = false;
private:
    QVector<QPixmap> m_animFrames;
    int m_currentFrameIndex;
    QTimer* m_animTimer;
private slots:
    void updateAnimation();
};

#endif // STATICCOIN_H
