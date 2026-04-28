#include "mainscene.h"

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int fontId = QFontDatabase::addApplicationFont(":/fonts/Fixedsys500c.ttf");
    MainScene w;
    w.show();
    return a.exec();
}
