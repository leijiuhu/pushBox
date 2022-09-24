#ifndef MAP_H
#define MAP_H
#include <QPainter>
#include <QObject>

enum
{
    Wall,//0墙
    Sky,//1路
    Box,//2箱子
    generalPoint,//3普通点
    winPoint,//4成功点
};

class Map : public QObject
{
    Q_OBJECT
public:
    explicit Map(QObject *parent = nullptr);
    void drawMap(QPainter *p,int p_x,int p_y);
    void init();
    ~Map();

signals:
public:
    int m_row;
    int m_col;
    int ** m_map;
    int mRank;//关数
};

#endif // MAP_H
