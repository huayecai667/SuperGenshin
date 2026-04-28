#include "mainscene.h"
#include "ui_mainscene.h"
#include "mypushbutton.h"
#include "playscene.h"
#include <QIcon>
#include<QPixmap>
#include<QPainter>
#include<QTimer>
MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    setFixedSize(448*3,224*3);
    setWindowIcon(QIcon(":/image/mario_0.png"));
    setWindowTitle("SuperMario");

    MyPushButton* startBtn=new MyPushButton(":/image/start.png");
    startBtn->setParent(this);
    startBtn->move(820,400);
    connect(startBtn,&MyPushButton::clicked,[=](){
        startBtn->zoom_s();
        startBtn->zoom_w();
        QTimer::singleShot(300,this,[=](){
            this->playscene=new PlayScene;
            this->hide();
            playscene->setGeometry(this->geometry());
            playscene->show();
        });
    });


    MyPushButton* helpBtn=new MyPushButton(":/image/help.png");
    helpBtn->setParent(this);
    helpBtn->move(820,480);
    connect(helpBtn,&MyPushButton::clicked,[=](){
        helpBtn->zoom_s();
        helpBtn->zoom_w();
        QTimer::singleShot(300,this,[=](){
            this->hide();
            this->helpscene=new HelpScene;
            helpscene->setGeometry(this->geometry());
            helpscene->show();
            connect(helpscene,&HelpScene::back,this,[=](){
                this->show();
            });
        });
    });

    MyPushButton* quitBtn=new MyPushButton(":/image/end.png");
    quitBtn->setParent(this);
    quitBtn->move(820,560);
    connect(quitBtn,&MyPushButton::clicked,[=](){
        quitBtn->zoom_s();
        quitBtn->zoom_w();
        QTimer::singleShot(300,this,[=](){
            this->close();
        });
    });



}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent*){
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/mainscene.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
