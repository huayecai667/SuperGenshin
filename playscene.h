#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include "player.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QFont>
#include <QGraphicsTextItem>
#include <QShortcut>

class PlayScene : public QWidget
{
    Q_OBJECT
public:
    explicit PlayScene(QWidget *parent = nullptr);
private:
    QGraphicsScene* m_scene; //游戏世界
    QGraphicsView* m_view;   //摄像机
    Player* m_player;
    QPixmap bgPixmap;

    static const int MAP_ROWS = 14;
    //222之后是地下场景
    static const int MAP_COLS = 212+38;
    int m_mapData[MAP_ROWS][MAP_COLS];
    QFont m_gameFont;
    QGraphicsTextItem* m_scoreText;
    QGraphicsTextItem* m_coinText;
    QGraphicsTextItem* m_timeText;
    QGraphicsTextItem* m_lifeText;
    int m_timeRemaining;
    bool m_isPaused;
    int m_frameCount;

    // 初始化场景
    void initScene();

    //初始化角色
    void initPlayer();

    // 初始化并解析地图数据的函数
    void generateMap();

    void updateCamera();



signals:
    void handleGameFailed();
};

#endif // PLAYSCENE_H
