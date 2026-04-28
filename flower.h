#ifndef FLOWER_H
#define FLOWER_H
#include"staticobject.h"
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QObject>
class Flower:public QObject,public StaticObject
{
    Q_OBJECT
public:
    explicit Flower(QGraphicsItem *parent = nullptr);
    virtual ~Flower() = default;
    virtual void onCollision(GameObject* other) override;
private:
    QVector<QPixmap> m_animFrames;
    int m_currentFrameIndex;
    QTimer* m_animTimer;


private slots:
    void updateAnimation();
};


#endif // FLOWER_H
