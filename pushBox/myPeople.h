#ifndef MYPEOPLE_H
#define MYPEOPLE_H
#include <QPainter>
#include <QPixmap>
class myPeople
{
public:
    myPeople();
    void drawPeople(QPainter *p,int p_x,int p_y);
    void peopleMove(int row,int col);
    QImage image;
    int mp_row;
    int mp_col;


};

#endif // MYPEOPLE_H
