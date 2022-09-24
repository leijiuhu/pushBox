#include "myPeople.h"
#include <QDebug>

myPeople::myPeople()
{
    //人的成员，位置，图片
    mp_row=2;//行
    mp_col=2;//列
    image =QImage(":/image/people.png");//初始化图片
}

void myPeople::drawPeople(QPainter *p,int p_x,int p_y)
{
//    qDebug()<<"行"<<mp_row;
//    qDebug()<<"列"<<mp_col;
    p->drawImage(QRect(p_x+image.width()*mp_col,
                  p_y+image.height()*mp_row,
                  image.width(),
                  image.height()),
                 image);
}

//人移动方法
void myPeople::peopleMove(int row,int col)
{
    mp_row =row;
    mp_col =col;
}
