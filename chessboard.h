#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "commdef.h"

class chessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit chessBoard(QWidget *parent = 0);
    //~chessBoard();

    void paintEvent(QPaintEvent *event);
    void boardInit();
    void mousePressEvent(QMouseEvent *event);
    void startGame();
    void clearSlc();//清空选择数组
    //void doMove(int direction);
    int fliped;
    QPointF points[6];  // 顶点数组
    QPointF sidepoints[6 * 3];//边点数组
    QPointF chesspoints[10][10];//棋盘实际坐标数组
    int chess[20][20];//棋盘抽象数组
    int chess_slc[15][15];
    int chess_move_count;
    Point_tomove poi_move[10];
    int gametype;//1PvP 0PvE

signals:
    void doMove(int direction);
public slots:

private:


};

#endif // CHESSBOARD_H
