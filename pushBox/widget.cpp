#include "widget.h"
#include <QDebug>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(800,600);

    StartBtn =new QPushButton(this);
    againBtn =new QPushButton(this);
    lastBtn =new QPushButton(this);
    nextBtn =new QPushButton(this);
    closeBtn =new QPushButton(this);

    StartBtn->setText("开始游戏");
    againBtn->setText("重新开始 ");
    lastBtn->setText("上一关");
    nextBtn->setText("下一关");
    closeBtn->setText("离开游戏");

    StartBtn->move(0,200);
    againBtn->move(0,300);
    lastBtn->move(0,400);
    nextBtn->move(0,500);
    closeBtn->move(0,600);

    //qt下设置按钮不接收键盘但能响应鼠标
    StartBtn->setFocusPolicy( Qt::NoFocus);
    againBtn->setFocusPolicy( Qt::NoFocus);
    lastBtn->setFocusPolicy( Qt::NoFocus);
    nextBtn->setFocusPolicy( Qt::NoFocus);
    closeBtn->setFocusPolicy( Qt::NoFocus);

    //设置背景透明
    StartBtn->setFlat(true);
    againBtn->setFlat(true);
    lastBtn->setFlat(true);
    nextBtn->setFlat(true);
    closeBtn->setFlat(true);

    setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("QPushButton{color:white; background-color:transparent;}");
    this->setWindowIcon(QIcon(":/image/pushBox.ico"));
    this->setWindowTitle(QString("推箱子"));

    //初始化参数
    m_start_X=0;
    m_start_Y=0;
    startFalg=false;
    startGameFalg =true;
    mMap = new Map(this);
    mPeople = new myPeople();
    updateTimer = new QTimer(this);
    updateTimer->start(100);

    connect(updateTimer,&QTimer::timeout,this,[=]
    {
        update();
    });

    //开始游戏(只能运行一次)
    connect(StartBtn,&QPushButton::clicked,this,[=]
    {
        if(startGameFalg == true)
        {
            mMap->init();
            startFalg = true;
            startGameFalg =false;
        }

    });
    //重新开始
    connect(againBtn,&QPushButton::clicked,this,[=]
    {
        mPeople->mp_row=2;//行
        mPeople->mp_col=2;//列
        mMap->init();
    });
    //上一个
    connect(lastBtn,&QPushButton::clicked,this,[=]
    {
        if(mMap->mRank >1)
        {
            mPeople->mp_row=2;//行
            mPeople->mp_col=2;//列
            mMap->mRank--;
            mMap->init();
        }
    });
    //下一个
    connect(nextBtn,&QPushButton::clicked,this,[=]
    {
        mPeople->mp_row=2;//行
        mPeople->mp_col=2;//列
        mMap->mRank++;
        mMap->init();
    });

    //离开游戏
    connect(closeBtn,&QPushButton::clicked,this,[=]
    {
        exit(0);
    });

}

Widget::~Widget()
{
    delete mPeople;
}

//绘图
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);

    const int post_x=width()/4;
    const int post_y=height()/4;


    //画背景图
    painter.drawImage(QRect(0,0,width(),height()),QImage(":/image/ground.png"));

    if(startFalg == true)
    {
        //画游戏地图
        mMap->drawMap(&painter,post_x,post_y);

        //画人
        mPeople->drawPeople(&painter,post_x,post_y);
    }
    painter.end();

}

//移动
void Widget::keyPressEvent(QKeyEvent *event)
{
    if(startFalg == true)
    {
        switch(event->key())
        {
        case Qt::Key_W:
        case Qt::Key_Up:
            moveCheck(-1,0);break;
        case Qt::Key_D:
        case Qt::Key_Right:
            moveCheck(0,1);;break;
        case Qt::Key_S:
        case Qt::Key_Down:
            moveCheck(1,0);;break;
        case Qt::Key_A:
        case Qt::Key_Left:
            moveCheck(0,-1);;break;
        }

        //计算是否过关
        QStringList strlist;
        for(int i=0;i<mMap->m_row;i++)
        {
            for(int j=0;j<mMap->m_col;j++)
            {
                strlist<<QString::number(mMap->m_map[i][j]) ;
            }
        }
        if(!strlist.contains(QString::number(generalPoint)))//如果不包含普通点游戏通过
        {
            qDebug()<<"恭喜，你赢了";

            //弹出游戏通关
            dialog =new QDialog ();
            dialog->setWindowIcon(QIcon(":/image/pushBox.ico"));
            dialog->setWindowTitle(QString("推箱子"));
            dialog->setStyleSheet("border-image:url(:/image/victory.png)");
            dialog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
            dialog->setModal(true); // 对话框特有
            dialog->setAttribute(Qt::WA_ShowModal, true); // 设置WA_ShowModal属性
            dialog->setWindowModality(Qt::ApplicationModal);//窗口是模态对于这个应用程序，并且阻塞应用程序内的其它所有窗口
            dialog->exec();
        }
    }
}

//移动检测
void Widget:: moveCheck(int row,int col)
{
    //移动之后的新位置
    int newRow =mPeople->mp_row+row;
    int newCol =mPeople->mp_col+col;
    if(mMap->m_map[newRow][newCol] == Wall)//人前面是墙
    {
        return;
    }
    else if(mMap->m_map[newRow][newCol] == Box)//人前面是箱子
    {
        if(mMap->m_map[newRow+row][newCol+col] == Sky)//箱子前面是路
        {
            mMap->m_map[newRow+row][newCol+col] =Box;//路的位置变成箱子
            mMap->m_map[newRow][newCol] =Sky;//箱子的位置变成路
        }else if(mMap->m_map[newRow+row][newCol+col] == generalPoint)//箱子前面是普通的点
        {
            mMap->m_map[newRow+row][newCol+col] =winPoint;//普通的点的位置变成箱子
            mMap->m_map[newRow][newCol] =Sky;//箱子的位置变成路
        }else
        {
            return;
        }
    }
    else if(mMap->m_map[newRow][newCol] == winPoint)//人前面是成功点
    {
        if(mMap->m_map[newRow+row][newCol+col] == Sky)//成功点前面是路
        {
            mMap->m_map[newRow+row][newCol+col] =Box;//路的位置变成箱子
            mMap->m_map[newRow][newCol] =generalPoint;//成功点的位置变成普通的点
        }else if(mMap->m_map[newRow+row][newCol+col] == generalPoint)//成功点前面是普通的点
        {
            mMap->m_map[newRow+row][newCol+col] =winPoint;//普通的点的位置变成功点
            mMap->m_map[newRow][newCol] =generalPoint;//成功点的位置变成普通的点
        }else
        {
            return;
        }
    }

    mPeople->peopleMove(newRow,newCol);

}
