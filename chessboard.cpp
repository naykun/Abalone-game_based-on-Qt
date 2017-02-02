#include "chessboard.h"
#include "mainwindow.h"
#include <QPainter>
#include <QtCore/QtMath>
//#include "comdef.h"

chessBoard::chessBoard(QWidget *parent) : QWidget(parent)
{
    resize(WIDGET_WIDTH,WIDGET_HEIGHT);
    move(WIDGETPOS_X,WIDGETPOS_Y);
//    startGame();
}
void chessBoard::boardInit(){
    for (int i = 0; i < COUNT; ++i) {
        float radians = 2 * M_PI / COUNT * i; // M_PI 是 QtMath 里定义的，就是 PI
        float x = RADIUS * qCos(radians);
        float y = -RADIUS * qSin(radians);
        points[i] = QPointF(x, y);
    }
    for (int i = 1; i <= COUNT ; ++i){
        sidepoints[(i-1)*3+2]=QPointF(points[i-1].rx()/4+(3*points[i%COUNT].rx())/4,points[i-1].ry()/4+(3*points[i%COUNT].ry())/4);
        sidepoints[(i-1)*3+1]=QPointF(2*points[i-1].rx()/4+2*(points[i%COUNT].rx())/4,2*points[i-1].ry()/4+2*(points[i%COUNT].ry())/4);
        sidepoints[(i-1)*3]=QPointF(3*points[i-1].rx()/4+1*(points[i%COUNT].rx())/4,3*points[i-1].ry()/4+1*(points[i%COUNT].ry())/4);
    }
    //fill chesspoints
    chesspoints[9][5] = points[0];
    chesspoints[5][9] = points[1];
    chesspoints[1][9] = points[2];
    chesspoints[1][5] = points[3];
    chesspoints[5][1] = points[4];
    chesspoints[9][1] = points[5];//polygon vertex
    chesspoints[8][6] = sidepoints[0];
    chesspoints[7][7] = sidepoints[1];
    chesspoints[6][8] = sidepoints[2];//side1
    chesspoints[4][9] = sidepoints[3];
    chesspoints[3][9] = sidepoints[4];
    chesspoints[2][9] = sidepoints[5];//side2
    chesspoints[1][8] = sidepoints[6];
    chesspoints[1][7] = sidepoints[7];
    chesspoints[1][6] = sidepoints[8];//side3
    chesspoints[2][4] = sidepoints[9];
    chesspoints[3][3] = sidepoints[10];
    chesspoints[4][2] = sidepoints[11];//side4
    chesspoints[6][1] = sidepoints[12];
    chesspoints[7][1] = sidepoints[13];
    chesspoints[8][1] = sidepoints[14];//side5
    chesspoints[9][2] = sidepoints[15];
    chesspoints[9][3] = sidepoints[16];
    chesspoints[9][4] = sidepoints[17];//side6
    for (int i=1;i <= 5; ++i){
        int d=5+i-2;
        double xbegin=chesspoints[5-i+1][i].rx(),xend=chesspoints[9][i].rx();
        double ybegin=chesspoints[5-i+1][i].ry();
        for(int j=5-i+2;j<=8;j++){
            chesspoints[j][i]=QPointF(((9-j)*xbegin+(d-9+j)*xend)/d,ybegin);
        }
    }//注意！！i为y，j为x
    for (int i=9;i >= 5; --i){
        int d=13-i;
        double xbegin=chesspoints[1][i].rx(),xend=chesspoints[14-i][i].rx();
        double ybegin=chesspoints[1][i].ry();
        for(int j=2;j<=13-i;j++){
            chesspoints[j][i]=QPointF(((d-j+1)*xbegin+(j-1)*xend)/d,ybegin);
        }
    }//注意！！i为y，j为x
    for(int i=1;i<=9;i++)
        for(int j=1;j<=9;j++)
            if(i+j>=6&&i+j<=14) chesspoints[i][j]-=QPointF(PIECES_RADIUS+4,PIECES_RADIUS+4);//修正坐标
}

void chessBoard::clearSlc(){
    memset(chess_slc,0,sizeof chess_slc);
}

void chessBoard::startGame(){
    boardInit();
    memset(chess,0,sizeof(chess));
    for(int i=5;i<=9;++i) chess[i][1]=1;
    for(int i=4;i<=9;++i) chess[i][2]=1;
    for(int i=5;i<=7;++i) chess[i][3]=1;
    for(int i=3;i<=5;++i) chess[i][7]=2;
    for(int i=1;i<=6;++i) chess[i][8]=2;
    for(int i=1;i<=5;++i) chess[i][9]=2;
    chess_move_count=0;
    fliped = 1;
    clearSlc();
}

void chessBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿
    painter.translate(width() / 2, height() / 2); // !important move the original point to the center of widget
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::cyan);
    for (int i = 0; i < COUNT; ++i) {
        float radians = 2 * M_PI / COUNT * i; // M_PI is defined in QtMath equals PI
        float x = BOARDRADIUS * qCos(radians);
        float y = -BOARDRADIUS * qSin(radians);
        points[i] = QPointF(x, y);
    }
    painter.drawPolygon(points, COUNT);//Black background

    painter.drawPixmap(BOARDPIX_X,BOARDPIX_Y,QPixmap(":/new/chessBoard/chessBoard3.png"));//chessBoardPix
    for(int i=1;i<=9;i++)
        for(int j=1;j<=9;j++)
            if(i+j>=6&&i+j<=14) {
                if(chess[i][j]==1){
                    if(chess_slc[i][j]==1)painter.drawPixmap(chesspoints[i][j],QPixmap(":/new/Pieces/black_ball_Fslc.png"));
                    else if(chess_slc[i][j]==2)painter.drawPixmap(chesspoints[i][j],QPixmap(":/new/Pieces/black_ball_slc.png"));
                    else painter.drawPixmap(chesspoints[i][j],QPixmap(":/new/Pieces/black_ball.png"));
                }
                if(chess[i][j]==2){
                    if(chess_slc[i][j]==1)painter.drawPixmap(chesspoints[i][j],QPixmap(":/new/Pieces/white_ball_Flc2.png"));
                    else if(chess_slc[i][j]==2)painter.drawPixmap(chesspoints[i][j],QPixmap(":/new/Pieces/white_ball_slc.png"));
                    else painter.drawPixmap(chesspoints[i][j],QPixmap(":/new/Pieces/white_ball.png"));
                }
            }
    painter.translate(-width() / 2,-height() / 2);//移回坐标系
//    painter.drawPixmap(chesspoints[5][5].rx()-PIECES_RADIUS-4,chesspoints[5][5].ry()-PIECES_RADIUS-4,QPixmap(":/new/Pieces/black_ball.png"));
//    painter.drawPixmap(chesspoints[6][5].rx()-PIECES_RADIUS-4,chesspoints[6][5].ry()-PIECES_RADIUS-4,QPixmap(":/new/Pieces/white_ball.png"));

/*  for (int i = 0; i < COUNT; ++i)
        painter.drawLine(points[i],QPointF(0,0));//draw radial line
*/

/*  for (int i = 1; i <= COUNT ; ++i) {
        painter.drawEllipse(sidepoints[(i-1)*3],45,45);
        painter.drawEllipse(sidepoints[(i-1)*3+1],45,45);
        painter.drawEllipse(sidepoints[(i-1)*3+2],45,45);
        painter.drawEllipse(points[i-1],45,45);
    }//draw outline points
*/
//  painter.drawEllipse(points[5],45,45);//draw test point
/*  painter.setBrush(Qt::white);
    for(int i=1;i<=9;i++)
        for(int j=1;j<=9;j++)
            if(i+j>=6&&i+j<=14) painter.drawEllipse(chesspoints[i][j],45,45);//draw inside points
*/

}


void chessBoard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        int row=0;
        int col=0;
        getPixmapIndex(event->pos().x(),event->pos().y(),row,col);
        if(chess_slc[col][row]==0&&chess[col][row] == fliped&&chess_move_count < 3) {
            poi_move[++chess_move_count].x = col;
            poi_move[chess_move_count].y = row;
            if(chess_move_count == 1) chess_slc[col][row]=1;
            else chess_slc[col][row]=2;
            }
        else {
            chess_move_count=0;
            clearSlc();
        }
        //emit doSlc();
        repaint();
    }
    if(event->button() == Qt::RightButton){
        int row=0;
        int col=0;
        getPixmapIndex(event->pos().x(),event->pos().y(),row,col);
        if(chess_move_count){
            int dict=getDirection(col,row,poi_move[1].x,poi_move[1].y);
            emit doMove(dict);//勿忘doMove后将chess_move_count置0
        }
    }
}
