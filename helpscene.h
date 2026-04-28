#ifndef HELPSCENE_H
#define HELPSCENE_H

#include <QWidget>
#include <QPixmap>
#include <QFont>
#include <QKeyEvent>
class HelpScene : public QWidget
{
    Q_OBJECT
public:
    explicit HelpScene(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap m_bgPixmap;
    QFont m_customFont;
signals:
    void back();
};

#endif // HELPSCENE_H
