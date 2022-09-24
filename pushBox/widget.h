#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "Map.h"
#include "myPeople.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    virtual void paintEvent(QPaintEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
private:
    void moveCheck(int row,int col);

private:
    Map *mMap;
    myPeople *mPeople;

    //人的开始位置
    int m_start_X;
    int m_start_Y;
    QDialog *dialog;
    QPushButton *StartBtn;
    QPushButton *againBtn;
    QPushButton *lastBtn;
    QPushButton *nextBtn;
    QPushButton *closeBtn;
    bool startFalg;
    bool startGameFalg;
    QTimer *updateTimer;
};
#endif // WIDGET_H
