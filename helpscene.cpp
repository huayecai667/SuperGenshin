#include "helpscene.h"
#include <QPainter>
#include <QFontDatabase>
#include <QIcon>
#include <mypushbutton.h>
#include<QTimer>
HelpScene::HelpScene(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(448*3,224*3);
    setWindowIcon(QIcon(":/image/mario_0.png"));
    setWindowTitle("Help");
    MyPushButton *back_btn = new MyPushButton(":/image/back.png");
    back_btn->setParent(this);
    back_btn->setFixedSize(180, 80);
    back_btn->setIconSize(QSize(180, 80));
    back_btn->move(820,560);
    connect(back_btn, &QPushButton::clicked, [=]() {
        back_btn->zoom_s();
        back_btn->zoom_w();
        QTimer::singleShot(500, this, [=]() {
            this->close();
            emit back();

        });
    });
    m_customFont = QFont("FixedsysTTF", 20);
}
void HelpScene::paintEvent(QPaintEvent *) {
    /*QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/mainscene.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    painter.setPen(Qt::black);
    painter.setFont(m_customFont);

    // painter.drawText(130, 60, "HOW TO PLAY");

    // painter.drawText(80, 100, "A / LEFT  :  MOVE LEFT");
    // painter.drawText(80, 130, "D / RIGHT :  MOVE RIGHT");
    // painter.drawText(80, 160, "W / SPACE :  JUMP");
    // painter.drawText(80, 160, "W / SPACE :  JUMP");
    painter.drawText(1344/2 - 100, 80, "HOW TO PLAY");

    // 基础移动操作（左对齐，x固定400，y依次递增35）
    painter.drawText(400, 130, "A / LEFT  :  MOVE LEFT");
    painter.drawText(400, 165, "D / RIGHT :  MOVE RIGHT");
    painter.drawText(400, 200, "W / SPACE :  JUMP");

    // 新增操作（和基础操作同列，间距一致）
    painter.drawText(400, 235, "C         :  SWITCH CHARACTER (MARIO/LUIGI)");
    painter.drawText(400, 270, "K         :  FIREBALL (FIRE FORM ONLY)");
    painter.drawText(400, 305, "P         :  PAUSE GAME");
    painter.drawText(400, 340, "R  */

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿，让矩形边缘更顺滑

    // 1. 绘制全屏背景图
    QPixmap pix;
    pix.load(":/image/mainscene.jpg");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);

    int bgWidth = 860;
    int bgHeight = 400;
    int bgX = (this->width() - bgWidth) / 2;
    int bgY = (this->height() - bgHeight) / 2;
    QRect textBgRect(bgX, bgY, bgWidth, bgHeight);
    painter.setPen(QColor(255, 255, 255, 200));
    painter.setBrush(QColor(255, 255, 255, 180));
    painter.drawRoundedRect(textBgRect, 10, 10);
    painter.setPen(Qt::black);
    painter.setFont(m_customFont);
    int titleX = bgX + (bgWidth - 160) / 2;
    painter.drawText(titleX, bgY + 60, "HOW TO PLAY");
    int textStartX = bgX + 100;
    int textStartY = bgY + 100;
    painter.drawText(textStartX, textStartY, "A / LEFT  :  MOVE LEFT");
    painter.drawText(textStartX, textStartY + 30, "D / RIGHT :  MOVE RIGHT");
    painter.drawText(textStartX, textStartY + 60, "W / SPACE :  JUMP");
    painter.drawText(textStartX, textStartY + 90, "C :  SWITCH CHARACTER (MARIO/LUIGI)");
    painter.drawText(textStartX, textStartY + 120, "K :  FIREBALL (FIRE FORM ONLY)");
    painter.drawText(textStartX, textStartY + 150, "P :  PAUSE GAME");
    painter.drawText(textStartX, textStartY + 180, "R :  RESTART GAME");
}
