#include "Map.h"
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMessageBox>
//地图类
Map::Map(QObject *parent)
    : QObject{parent}
{
    mRank =1;
}

void Map::init()
{
    QString filename =QCoreApplication::applicationDirPath();
    filename +=QString("/file/map%1.bin").arg(mRank);
    qDebug()<<filename;
    QFile file(filename);
    if(!file.exists())
    {
        qDebug()<<"初始化关卡错误";
        QMessageBox::warning(nullptr,QString("错误提示"),QString("警告已经到最后一关!"));
        mRank--;
        return;
    }
    file.open(QIODevice::ReadOnly);
    QString strData=file.readAll();

    QStringList listStr= strData.split("\r\n");
    m_row = listStr.size();//确定行数
    m_map =new int*[m_row];
    for(int i=0;i<m_row;i++)
    {
       QStringList tempStrList= listStr.at(i).split(",");
       m_col =tempStrList.size();//确定列数
       m_map[i]=new int [m_col];
       for(int j=0;j<m_col;j++)
       {
           m_map[i][j]=tempStrList.at(j).toInt();//保存每个数据
       }
    }

#if 0
    //打印数组元素
    for(int i=0;i<m_row;i++)
    {
        QStringList list;
       for(int j=0;j<m_col;j++)
       {
           list.push_back(QString::number(m_map[i][j]));
       }
       qDebug()<<list;
       list.clear();
    }
#endif
    file.close();
}

//参数1: 画家
//参数2: 画的起始位置x方向
//参数3: 画的起始位置y方向
void Map::drawMap(QPainter *p,int p_x,int p_y)
{
    //画游戏地图
    for(int i=0;i<m_row;i++)
    {
        for(int j=0;j<m_col;j++)
        {
            QString url;
            switch(m_map[i][j])
            {
            case Wall:url=":/image/wall.png";break;
            case Sky:url=":/image/sky.png";break;
            case Box:url=":/image/case.png";break;
            case generalPoint:url=":/image/end.png";break;
            case winPoint:url=":/image/win.png";break;
            }
            QImage image(url);//初始化图片
            //p->drawImage(p_x+j*pixmap.width(),p_y+i*pixmap.height(),pixmap.width(),pixmap.height(),pixmap);
            p->drawImage(QRect(p_x+j*image.width(),p_y+i*image.height(),image.width(),image.height()),image);
        }
    }
}

Map::~Map()
{
    //手动释放数组
    for(int i=0;i<m_row;i++)
    {
        delete [] m_map[m_col];
        delete m_map[i];
    }
}
