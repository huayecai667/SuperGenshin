#include "playscene.h"
#include "brick.h"
#include "enemy.h"
#include "pipe.h"
#include "castle.h"
#include "flagpole.h"
#include "flag.h"
#include "piranhaplant.h"
#include "staticcoin.h"
#include "soundmanager.h"
#include<QDebug>
#include<QIcon>
#include <QPainter>
#include <QTimer>
#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsView>
#include <QString>
PlayScene::PlayScene(QWidget *parent)
    : QWidget{parent},
    bgPixmap(":/image/background.png"),
    m_timeRemaining(300),
    m_isPaused(false),
    m_frameCount(0)
{
    setFixedSize(448*3,224*3);
    setWindowIcon(QIcon(":/image/mario_0.png"));
    setWindowTitle("SuperMario");
    initScene();
    generateMap();
    initPlayer();
    SoundManager::instance().init();
    SoundManager::instance().playBGM();
    QTimer::singleShot(180,this,[=](){
        SoundManager::instance().playBGM();
    });
}

void PlayScene::initScene()
{

    this->m_scene = new QGraphicsScene(this);
    int worldWidth = MAP_COLS * 16;
    int worldHeight = 224;
    m_scene->setSceneRect(0, 0, worldWidth, worldHeight);
    m_scene->setBackgroundBrush(QBrush(bgPixmap));
    //游戏视图的性能和视觉优化
    this->m_view = new QGraphicsView(m_scene, this);
    m_view->setFrameStyle(QFrame::NoFrame);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing);
    m_view->scale(3.0, 3.0);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // 消除四周的白边
    layout->addWidget(m_view);
    this->setLayout(layout);

    m_gameFont = QFont("FixedsysTTF", 7);
    //初始化得分文本
    m_scoreText = new QGraphicsTextItem("SCORE\n00000");
    m_scoreText->setFont(m_gameFont);
    m_scoreText->setDefaultTextColor(Qt::white);
    m_scoreText->setZValue(100);
    m_view->scene()->addItem(m_scoreText);
    //初始化金币文本
    m_coinText = new QGraphicsTextItem("COINS\n 00");
    m_coinText->setFont(m_gameFont);
    m_coinText->setDefaultTextColor(Qt::white);
    m_coinText->setZValue(100);
    m_view->scene()->addItem(m_coinText);
    //初始化时间文本
    m_timeText = new QGraphicsTextItem(QString("TIME\n%1").arg(m_timeRemaining,3, 10, QChar('0')));
    m_timeText->setFont(m_gameFont);
    m_timeText->setDefaultTextColor(Qt::white);
    m_timeText->setZValue(100);
    m_view->scene()->addItem(m_timeText);
    //初始化生命文本
    m_lifeText = new QGraphicsTextItem("LIVES\n  3");
    m_lifeText->setFont(m_gameFont);
    m_lifeText->setDefaultTextColor(Qt::white);
    m_lifeText->setZValue(100);
    m_view->scene()->addItem(m_lifeText);

    QShortcut *pauseShortcut = new QShortcut(QKeySequence(Qt::Key_P), this);
    connect(pauseShortcut, &QShortcut::activated, this, [=](){
        if(m_isPaused){
            m_isPaused = !m_isPaused;
            SoundManager::instance().playBGM();
        }
        else{
            m_isPaused = !m_isPaused;
            SoundManager::instance().stopBGM();
        }
    });
    QShortcut *restartShortcut = new QShortcut(QKeySequence(Qt::Key_R), this);
    connect(restartShortcut, &QShortcut::activated, this, [=]() {
        if (!m_isPaused) {
            m_isPaused = true;
        }

        SoundManager::instance().stopBGM();
        PlayScene* newGame = new PlayScene();
        newGame->show();
        this->close();
        this->deleteLater();
    });



    QTimer *gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [=]() {
        if(m_isPaused)
            return;
        // 每一帧，遍历场景里所有的 GameObject，让它们更新逻辑
        for (QGraphicsItem *item : m_scene->items()) {
            GameObject *obj = dynamic_cast<GameObject*>(item);
            if (obj) {
                obj->updateLogic();
            }
        }
        if(m_player->m_isUnder){
            m_scene->setBackgroundBrush(Qt::black);
        }
        if(m_player->x()>=3920&&(qAbs(m_player->y() - 177) < 2.0 || qAbs(m_player->y() - 161) < 2.0 )){
            QTimer::singleShot(500,this,[=](){
                m_scene->setBackgroundBrush(QBrush(bgPixmap));
                m_player->m_isUnder=false;
                SoundManager::instance().playSound("pipe");
                if(m_player->getPlayerForm()==PlayerForm::Small)
                    m_player->setPos(904, 113);
                else
                    m_player->setPos(904, 97);
            });
        }

        if(m_player->getLives() <= 0) {
            SoundManager::instance().stopBGM();
            SoundManager::instance().playSound("fail");
            emit m_player->gameFailed(1);
            return;
        }
        m_frameCount++;
        if (m_frameCount >= 60) {
            m_frameCount = 0;
            m_timeRemaining--;

            // 更新UI上的数字
            m_timeText->setPlainText(QString("TIME\n%1").arg(m_timeRemaining, 3, 10, QChar('0')));

            if (m_timeRemaining <= 0) {
                m_timeRemaining = 0;
                SoundManager::instance().stopBGM();
                SoundManager::instance().playSound("time");
                emit m_player->gameFailed(2);
                return;
            }
        }
        this->updateCamera();
    });

    gameTimer->start(16);
}



void PlayScene::initPlayer(){
    this->m_player=new Player();
    m_player->setPos(50, 175);
    m_scene->addItem(m_player);
    this->updateCamera();
    // 获取键盘焦点
    m_player->setFocus();

    QObject::connect(m_player, &Player::scoreChanged, this, [=](int newScore){
        m_scoreText->setPlainText(QString("SCORE\n%1").arg(newScore, 5, 10, QChar('0')));
    });

    QObject::connect(m_player, &Player::coinChanged, this, [=](int newCoin){
        m_coinText->setPlainText(QString("COINS\n  %1").arg(newCoin, 2, 10, QChar('0')));
    });
    QObject::connect(m_player, &Player::lifeChanged, this, [=](int newlife){
        m_lifeText->setPlainText(QString("LIVES\n  %1").arg(newlife, 2, 10, QChar('0')));
    });

    connect(m_player, &Player::gameWon, this, [=]() {
        if (m_isPaused) return;
        m_isPaused = true;
        if (m_timeRemaining > 0) {
            QTimer* scoreTimer = new QTimer(this);
            connect(scoreTimer, &QTimer::timeout, this, [=]() {
                if (m_timeRemaining > 0) {
                    m_timeRemaining--;
                    m_player->addScore(50);
                    emit m_player->scoreChanged(m_player->getScore());
                    m_timeText->setPlainText(QString("TIME\n%1").arg(m_timeRemaining, 3, 10, QChar('0')));
                } else {
                    scoreTimer->stop();
                    scoreTimer->deleteLater();
                }
            });
            scoreTimer->start(15);
        }
    });


    connect(m_player, &Player::gameFailed, this, [=](int failKind) {
        if (m_isPaused) return;
        m_isPaused = true;
        QGraphicsPixmapItem* gameOverImg = new QGraphicsPixmapItem(QPixmap(":/image/game_over.png"));
        if(failKind == 2) {
            gameOverImg->setPixmap(QPixmap(":/image/time_up.png"));
        }
        QPointF viewCenter = m_view->mapToScene(m_view->viewport()->rect().center());
        gameOverImg->setPos(viewCenter.x() - gameOverImg->boundingRect().width() / 2,
                            viewCenter.y() - gameOverImg->boundingRect().height() / 2);
        gameOverImg->setZValue(100);
        m_scene->addItem(gameOverImg);
    });

}

void PlayScene::updateCamera()
{
    if (!m_player) return;
    //获取马里奥当前的x坐标
    qreal playerX = m_player->x();
    //获取当前视口的逻辑中心y坐标
    qreal fixedY = m_scene->sceneRect().center().y();

    //让摄像机的中心对准马里奥的x坐标
    m_view->centerOn(playerX, fixedY);

    QRectF visibleRect = m_view->mapToScene(m_view->viewport()->rect()).boundingRect();
    if (m_scoreText)
        m_scoreText->setPos(visibleRect.left(), visibleRect.top()+5);
    if (m_coinText)
        m_coinText->setPos(visibleRect.center().x()-90, visibleRect.top()+5);
    if (m_timeText)
        m_timeText->setPos(visibleRect.center().x()+40, visibleRect.top()+5);
    if (m_lifeText)
        m_lifeText->setPos(visibleRect.right() - 70, visibleRect.top()+5);

}



void PlayScene::generateMap()
{
    for (int r = 0; r < MAP_ROWS; ++r) {
        for (int c = 0; c < MAP_COLS; ++c) {
            m_mapData[r][c] = 0;
        }
    }
    for (int c = 0; c < 222; ++c) {
        m_mapData[MAP_ROWS - 2][c] = 1;
        m_mapData[MAP_ROWS - 1][c] = 1;
    }
    for(int c=222;c<MAP_COLS;++c){
        m_mapData[MAP_ROWS - 2][c] = 11;
        m_mapData[MAP_ROWS - 1][c] = 11;
    }

    m_mapData[8][16] = 3;


    m_mapData[8][20] = 2;
    m_mapData[8][21] = 15;
    m_mapData[8][22] = 2;
    m_mapData[4][22] = 3;
    m_mapData[8][23] = 3;
    m_mapData[8][24] = 2;



    m_mapData[10][27]=6;
    m_mapData[9][37]=6;
    m_mapData[8][45]=6;
    m_mapData[8][56]=6;


    m_mapData[11][17]=5;
    m_mapData[11][32]=5;
    m_mapData[11][40]=5;
    m_mapData[11][52]=5;
    m_mapData[11][53]=5;

    m_mapData[12][65]=0;
    m_mapData[12][66]=0;
    m_mapData[13][65]=0;
    m_mapData[13][66]=0;

    m_mapData[8][76]=2;
    m_mapData[8][77]=16;
    m_mapData[8][78]=2;

    m_mapData[4][79]=2;
    m_mapData[4][80]=2;
    m_mapData[4][81]=2;
    m_mapData[4][82]=2;
    m_mapData[4][83]=2;
    m_mapData[4][84]=2;
    m_mapData[4][85]=2;

    m_mapData[7][77]=5;
    m_mapData[3][84]=5;



    m_mapData[12][82]=0;
    m_mapData[12][83]=0;
    m_mapData[13][82]=0;
    m_mapData[13][83]=0;

    m_mapData[4][88]=2;
    m_mapData[4][89]=2;
    m_mapData[4][90]=3;
    m_mapData[8][90]=3;

    m_mapData[11][90]=5;
    m_mapData[11][91]=5;

    m_mapData[8][94]=2;
    m_mapData[8][95]=2;

    m_mapData[8][99]=3;
    m_mapData[8][101]=3;
    m_mapData[4][101]=3;
    m_mapData[8][103]=3;

    m_mapData[8][108]=2;

    m_mapData[4][111]=2;
    m_mapData[4][112]=2;
    m_mapData[4][113]=2;

    m_mapData[11][112]=5;
    m_mapData[11][113]=5;

    m_mapData[4][117]=2;
    m_mapData[4][118]=3;
    m_mapData[4][119]=3;
    m_mapData[4][120]=2;

    m_mapData[8][118]=2;
    m_mapData[8][119]=2;

    for(int j=11;j>=8;--j){
        int strI=121+(11-j);
        for(int i=strI;i<=124;++i){
            m_mapData[j][i]=7;
        }
    }

    for(int j=11;j>=8;--j){
        int endI=130-(11-j);
        for(int i=127;i<=endI;++i){
            m_mapData[j][i]=7;
        }
    }

    for(int j=11;j>=8;--j){
        int strI=133+(11-j);
        for(int i=strI;i<=137;++i){
            m_mapData[j][i]=7;
        }
    }
    m_mapData[12][138]=0;
    m_mapData[12][139]=0;
    m_mapData[13][138]=0;
    m_mapData[13][139]=0;

    for(int j=11;j>=8;--j){
        int endI=143-(11-j);
        for(int i=140;i<=endI;++i){
            m_mapData[j][i]=7;
        }
    }

    m_mapData[11][153]=7;

    m_mapData[11][155]=7;
    m_mapData[10][155]=7;

    m_mapData[11][157]=7;
    m_mapData[10][157]=7;
    m_mapData[9][157]=7;

    m_mapData[11][159]=7;
    m_mapData[10][159]=7;
    m_mapData[9][159]=7;
    m_mapData[8][159]=7;

    m_mapData[11][161]=7;
    m_mapData[10][161]=7;
    m_mapData[9][161]=7;
    m_mapData[8][161]=7;

    m_mapData[11][163]=7;
    m_mapData[10][163]=7;
    m_mapData[9][163]=7;

    m_mapData[11][165]=5;
    m_mapData[7][165]=2;

    m_mapData[11][167]=7;
    m_mapData[10][167]=7;
    m_mapData[9][167]=7;

    m_mapData[11][169]=7;
    m_mapData[10][169]=7;




    m_mapData[10][172]=6;

    m_mapData[8][176]=2;
    m_mapData[8][177]=2;
    m_mapData[8][178]=3;
    m_mapData[8][179]=2;

    m_mapData[11][176]=5;


    m_mapData[10][183]=6;
    for(int j=11;j>=4;--j){
        int strI=185+(11-j);
        for(int i=strI;i<=193;++i){
            m_mapData[j][i]=7;
        }
    }


    //城堡、旗帜和旗杆
    m_mapData[11][199]=7;
    m_mapData[1][199]=9;
    m_mapData[2][198]=10;
    m_mapData[7][204]=8;


    for(int i=1;i<=11;++i){
        m_mapData[i][235]=12;
    }
    for(int i=238;i<=247;++i){
        m_mapData[0][i]=12;
        m_mapData[1][i]=12;
    }
    for(int i=11;i>=9;--i){
        for(int j=238;j<=243;++j){
            m_mapData[i][j]=12;
        }
    }
    for(int i=8;i>=6;--i){
        for(int j=238;j<=243;++j){
            m_mapData[i][j]=13;
        }
    }
    m_mapData[10][246]=14;
    m_mapData[0][248]=6;





    //根据数组生成对象
    for (int r = 0; r < MAP_ROWS; ++r) {
        for (int c = 0; c < MAP_COLS; ++c) {

            int type = m_mapData[r][c];
            if (type == 0) continue;

            //计算在场景里的绝对像素坐标
            int pixelX = c * 16;
            int pixelY = r * 16;

            if (type == 1) {
                Brick *ground = new Brick(BrickType::Ground);
                ground->setPos(pixelX, pixelY);
                m_scene->addItem(ground);
            }
            else if (type == 2) {
                Brick *brick = new Brick(BrickType::BreakableBrick);
                brick->setPos(pixelX, pixelY);
                m_scene->addItem(brick);
            }
            else if(type==3){
                Brick *brick = new Brick(BrickType::QuestionBlock);
                brick->setPos(pixelX, pixelY);
                m_scene->addItem(brick);
            }
            else if(type==5){
                Enemy* enemy=new Enemy();
                enemy->setPos(pixelX, pixelY);
                m_scene->addItem(enemy);
            }
            else if(type==6){
                Pipe* pipe=new Pipe(12-r);
                pipe->setPos(pixelX,pixelY);
                m_scene->addItem(pipe);

                PiranhaPlant* piranha = new PiranhaPlant(pixelX + 16, pixelY);
                m_scene->addItem(piranha);

            }
            else if(type==7){
                Brick *brick = new Brick(BrickType::UnbreakableBrick);
                brick->setPos(pixelX, pixelY);
                m_scene->addItem(brick);
            }
            else if(type==8){
                Castle* castle=new Castle();
                castle->setPos(pixelX, pixelY);
                m_scene->addItem(castle);
            }
            else if(type==9){
                FlagPole* flagpole=new FlagPole();
                flagpole->setPos(pixelX+4, pixelY+8);
                m_scene->addItem(flagpole);
            }
            else if(type==10){
                Flag* flag=new Flag();
                flag->setPos(pixelX+8, pixelY);
                m_scene->addItem(flag);
            }
            else if(type==11){
                Brick *brick = new Brick(BrickType::UnderGround);
                brick->setPos(pixelX, pixelY);
                m_scene->addItem(brick);
            }
            else if(type==12){
                Brick *brick = new Brick(BrickType::UnderBrick);
                brick->setPos(pixelX, pixelY);
                m_scene->addItem(brick);
            }
            else if(type==13){
                StaticCoin* coin=new StaticCoin();
                coin->setPos(pixelX, pixelY);
                m_scene->addItem(coin);
            }
            else if(type==14){
                Pipe* pipe=new Pipe();
                pipe->setPos(pixelX,pixelY);
                m_scene->addItem(pipe);
            }
            else if(type==15){
                Brick *brick = new Brick(BrickType::QuestionBlock,1);
                brick->setPos(pixelX, pixelY);
                m_scene->addItem(brick);
            }
            else if(type==16){
                Brick *brick = new Brick(BrickType::QuestionBlock,2);
                brick->setPos(pixelX, pixelY);
                m_scene->addItem(brick);
            }
        }
    }

}
