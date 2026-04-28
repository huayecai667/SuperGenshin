#ifndef ENEMY_H
#define ENEMY_H
#include "dynamicobject.h"
#include <QObject>
class Enemy:public QObject,public DynamicObject
{
    Q_OBJECT
public:
    explicit Enemy(QGraphicsItem* parent=nullptr);
    virtual ~Enemy()=default;

    virtual void updateLogic() override;
    virtual void onCollision(GameObject* other) override;
    bool isDead() const { return m_isDead; }
    void die();
private:
    void resolveTerrainCollisions(Qt::Orientation orientation);
    bool m_isActive;
    QVector<QPixmap> m_animFrames;
    int m_animDelay = 0;
    int m_currentFrameIndex=0;
    bool m_isDead = false;
    GameObject* m_playerTarget = nullptr;
};

#endif // ENEMY_H
