#ifndef COMMDEF_H
#define COMMDEF_H
#include <QObject>
#include <QtCore/QtMath>

const int PIECES_RADIUS = 45;
const int WIDGET_WIDTH = 1100;
const int WIDGET_HEIGHT = 1000;
const int WIDGETPOS_X = 0;
const int WIDGETPOS_Y = 0;
const int COUNT  = 6;  // 边数
const int RADIUS = 450; // 棋子阵的半径
const int BOARDRADIUS = 550;//棋盘的半径
const int BOARDPIX_X = -(RADIUS+115);//棋盘图片x坐标
const int BOARDPIX_Y = -(RADIUS+33);//棋盘图片y坐标
const int v1[7]={0,0,0,1,-1,1,-1};//directions
const int v2[7]={0,1,-1,0,0,-1,1};
struct Point_tomove{
    int x,y;
    bool operator <(const Point_tomove &a) const {
        return x!=a.x ? x<a.x : y<a.y;
    }
};
typedef struct Point_tomve Point_tomve;

/*void swap(Point_tomove &a,Point_tomove &b){
    Point_tomove t=a; a=b; b=t;
}

void swap(int &a, int &b){
    int t=a; a=b; b=t;
}
*/

inline void getPixmapIndex(int xx,int yy,int &row,int &col)
{
    row = (WIDGET_HEIGHT-yy-45)/(2*PIECES_RADIUS+10)+1;
    col = (xx-int((row*100)*qTan(M_PI/6))+220)/(2*PIECES_RADIUS+20)+1;

    return;
}
inline int getDirection(int x1,int y1,int x2,int y2){//x->col y->row
    if(x1==x2){
        if(y1>y2) return 1;//up
        else if(y1<y2) return 2;//down
    }
    else if(y1==y2){
        if(x1>x2) return 3;//right
        else if(x1<x2) return 4;//left
    }
    else if(y1>y2&&x1<x2) return 6;//leftup
    else if(y1<y2&&x1>x2) return 5;//rightdown
    else if(x1==x2&&y1==y2) return 0;
    return 0;
}
inline bool In_chessboard(int i,int j) {
    return i>0 && j>0 && i<10 && j<10 && i+j>=6 && i+j<=14;
}





#endif // COMMDEF_H
