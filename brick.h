#ifndef BRICK_H
#define BRICK_H
#include"staticobject.h"
#include <QPixmap>
#include <QVector>
#include <QTimer>
#include <QObject>
enum class BrickType{
    Ground,
    BreakableBrick,
    UnbreakableBrick,
    QuestionBlock,
    UsedQuestionBlock,
    UnderGround,
    UnderBrick
};

class Brick:public QObject,public StaticObject
{
    Q_OBJECT
public:
    explicit Brick(BrickType type, int unkown=0,QGraphicsItem *parent = nullptr);
    virtual ~Brick() = default;
    BrickType getBrickType() const { return m_brickType; }
    void transformToUsed();
    void brickBreak();
    virtual void onCollision(GameObject* other) override;
private slots:
    void updateAnimation();
private:
    BrickType m_brickType;
    int unkowntype;
    QVector<QPixmap> m_animFrames; //存储动画帧
    int m_currentFrameIndex;       //当前播放到了第几帧
    QTimer* m_animTimer;           //控制动画播放的定时器
};

#endif // BRICK_H
