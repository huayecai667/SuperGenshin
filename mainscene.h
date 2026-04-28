#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include "playscene.h"
#include "helpscene.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QWidget
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();
    PlayScene* playscene=nullptr;
    HelpScene* helpscene=nullptr;

    void paintEvent(QPaintEvent*);

private:
    Ui::MainScene *ui;
};
#endif // MAINSCENE_H
