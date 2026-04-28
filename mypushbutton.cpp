#include "mypushbutton.h"
#include<QPropertyAnimation>
#include<QDebug>
MyPushButton::MyPushButton(QString normalImage, QString pressImage)
{
    this->normalImgPath = normalImage;
    this->pressImgPath = pressImage;

    QPixmap pix;
    bool ret = pix.load(normalImage); //默认显示normalImage图片
    if(!ret)
    {
        qDebug() << "图片加载失败";
        return;
    }

    //设置图片固定大小
    this->setFixedSize(200, 100);
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px}");
    //设置图标
    this->setIcon(pix);
    //设置图标大小
    this->setIconSize(QSize(200, 100));

}
void MyPushButton::zoom_s(){
    QPropertyAnimation* animation=new QPropertyAnimation(this,"geometry");
    animation->setDuration(200);
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
void MyPushButton::zoom_w(){
    QPropertyAnimation* animation=new QPropertyAnimation(this,"geometry");
    animation->setDuration(200);
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
void MyPushButton::mousePressEvent(QMouseEvent* e){
    if(this->pressImgPath!=""){
        QPixmap pix;
        bool ret = pix.load(this->pressImgPath);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }

        //设置图片固定大小
        this->setFixedSize(200, 100);
        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px}");
        //设置图标
        this->setIcon(pix);
        //设置图标大小
        this->setIconSize(QSize(200, 100));
    }
    return QPushButton::mousePressEvent(e);
}
void MyPushButton::mouseReleaseEvent(QMouseEvent* e){
    if(this->pressImgPath!=""){
        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret)
        {
            qDebug() << "图片加载失败";
            return;
        }

        //设置图片固定大小
        this->setFixedSize(200, 100);
        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px}");
        //设置图标
        this->setIcon(pix);
        //设置图标大小
        this->setIconSize(QSize(200, 100));
    }
    return QPushButton::mouseReleaseEvent(e);
}
