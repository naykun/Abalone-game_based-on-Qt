#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <Qpen>
#include <QtCore/qmath.h>
#include <algorithm>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    freopen("test.txt","w",stdout);


    ui->setupUi(this);
    chessboard = new chessBoard(this);
    ui->retract->raise();
    initActions();
    startgame();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::initActions(){
    connect(chessboard,SIGNAL(doMove(int)),this,SLOT(doMove(int)));
    connect(ui->retract,SIGNAL(clicked(bool)),this,SLOT(retract()));
}
void MainWindow::swap(Point_tomove &a,Point_tomove &b){
    Point_tomove t=a; a=b; b=t;
}
void MainWindow::swap(int &a, int &b){
    int t=a; a=b; b=t;
}
void MainWindow::startgame(){
    black_out=white_out=win=0;
    chessboard->startGame();
    calculate();
    chessboard->repaint();
    chessboard->fliped=1;
    for(int i=1;i<=9;++i)
            for(int j=1;j<=9;++j)
            if(i+j>=6 && i+j<=14) {
                chess_laststate[i][j]=chessboard->chess[i][j];
            }
    black_out_laststate=black_out;
    white_out_laststate=white_out;
    lastfliped=chessboard->fliped;
}
void MainWindow::calculate(){
   // if(chessboard->gametype!=0) printf("cal unstarted\n");
    chessboard->fliped=3-chessboard->fliped;
    int tmp[20][20];
    memset(tmp,0,sizeof(tmp));
   // if(chessboard->gametype!=0) printf("cal memset\n");
    int black_num=0,white_num=0;
    //
   // if(chessboard->gametype!=0) printf("cal 1\n");
    //
    for(int i=1;i<=9;++i)
    for(int j=1;j<=9;++j)
    if(i+j>=6 && i+j<=14) {
        if(chessboard->chess[i][j]==1) ++black_num;
        if(chessboard->chess[i][j]==2) ++white_num;
        tmp[i][j]=chessboard->chess[i][j];
    }
    memcpy(chessboard->chess,tmp,sizeof(tmp));
    white_out=14-white_num;
    ui->white_out->display(white_out);
    black_out=14-black_num;
    ui->black_out->display(black_out);
    //
   // if(chessboard->gametype!=0) printf("cal 2\n");
    //
    if(white_out>=6) {
        win=1;
        QMessageBox message(QMessageBox::NoIcon, "黑棋赢！", "黑棋胜利！",QMessageBox::Yes);
        message.setIconPixmap(QPixmap(":/new/Pieces/black_ball_Fslc.png"));
        if(message.exec() == QMessageBox::Yes)
        {
            startgame();
        }
        //message.exec();
    }//black
    if(black_out>=6) {
        win=2;
        QMessageBox message(QMessageBox::NoIcon, "白棋赢！", "白棋胜利！",QMessageBox::Yes);
        message.setIconPixmap(QPixmap(":/new/Pieces/white_ball_Flc2.png"));
        if(message.exec() == QMessageBox::Yes)
        {
            startgame();
        }
        //message.exec();
    }//white
}
void MainWindow::retract(){
    for(int i=1;i<=9;++i)
            for(int j=1;j<=9;++j)
            if(i+j>=6 && i+j<=14) {
                chessboard->chess[i][j]=chess_laststate[i][j];
            }
    black_out=black_out_laststate;
    white_out=white_out_laststate;
    calculate();
    chessboard->fliped=lastfliped;
    chessboard->repaint();
}

void MainWindow::work(int direction){
    if(chessboard->gametype==1||chessboard->fliped==1){
        for(int i=1;i<=9;++i)
                for(int j=1;j<=9;++j)
                if(i+j>=6 && i+j<=14) {
                    chess_laststate[i][j]=chessboard->chess[i][j];
                }
        black_out_laststate=black_out;
        white_out_laststate=white_out;
        lastfliped=chessboard->fliped;
    }
    if(chessboard->gametype!=0) printf("work 0\n");
    for(int i=chessboard->chess_move_count;i>0;--i) {
                int x=chessboard->poi_move[i].x,y=chessboard->poi_move[i].y;
                swap(chessboard->chess[x][y],chessboard->chess[x+v1[direction]][y+v2[direction]]);
            }
    if(chessboard->gametype!=0) printf("work 1\n");
    chessboard->chess_move_count=0;
    chessboard->clearSlc();
    chessboard->repaint();
    calculate();
    //
    if(chessboard->gametype!=0) printf("work 2\n");
    //
    if(chessboard->gametype==0&&chessboard->fliped==2){
        AI_move();
    }
    if(chessboard->gametype!=0) printf("work 3\n");

    /*   for(int i=1;i<=9;i++){
           for(int j=1;j<=9;j++){
               if(i+j>=6&&i+j<=14) printf("%d",chessboard->chess[i][j]);
               else printf(" ");
           }
           printf("\n");
       }*/
}

void MainWindow::assign(chess_board_tmp &a) {
    a.Twhiteout = white_out;
    a.Tblackout = black_out;
    a.move = chessboard->fliped;
//	a->win = chessboard->win;
    a.tot = 0;
    a.direction = 0;
    memcpy(a.chess,chessboard->chess,sizeof(a.chess));
//	memcpy(a->poi,chessboard->poi_move,sizeof(a->poi_move));
}

int MainWindow::AI_check_danger(const chess_board_tmp &tmp, int flipped){
    int counter = 0;
    for(int i=5;i<=9;++i) {
        if(tmp.chess[i][1]==flipped) {
            int x=i,y=1,cnt[10],dir=1;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
//---------------------------------------------------------------------------------------------------
            x=i,y=1,dir=6;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
        }
    }
    for(int i=1;i<=5;++i) {
        if(tmp.chess[i][9]==flipped) {
            int x=i,y=9,cnt[10],dir=2;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
//---------------------------------------------------------------------------------------
            x=i,y=9,dir=5;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
        }
    }
    for(int j=5;j<=9;++j) {
        if(tmp.chess[1][j]==flipped) {
            int x=1,y=j,cnt[10],dir=3;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
//-----------------------------------------------------------------------------------
            x=1,y=j,dir=5;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
        }
    }
    for(int j=1;j<=5;++j) {
        if(tmp.chess[9][j]==flipped) {
            int x=9,y=j,cnt[10],dir=4;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
//-----------------------------------------------------------------------------------------
            x=9,y=j,dir=6;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
        }
    }
    for(int i=1;i<=5;++i) {// start from 1,5 -> 2,4
        int j=6-i;
        if(tmp.chess[i][j]==flipped) {
            int x=i,y=j,cnt[10],dir=3;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
//-----------------------------------------------------------------------------------------------------
            x=i,y=j,dir=1;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
        }
    }
    for(int i=5;i<=9;++i) {// start from 5,9 -> 6,8
        int j=14-i;
        if(tmp.chess[i][j]==flipped) {
            int x=i,y=j,cnt[10],dir=2;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
//--------------------------------------------------------------------------------------
            x=i,y=j,dir=4;
            memset(cnt,0,sizeof(cnt));
            for(int k=1;k<=5;++k) {
                cnt[k]=tmp.chess[x][y];
                x+=v1[dir];
                y+=v2[dir];
            }
            if(cnt[2]==3-flipped && cnt[3]==cnt[2]) ++counter;
            else if(cnt[2]==flipped && cnt[3]==3-flipped && cnt[4]==cnt[3] && cnt[5]==cnt[4])
                ++counter;
        }
    }
    return counter;
}

void MainWindow::AI_trymove(int now){
    chess_board_tmp temp,tmp;
    assign(temp);
    Total=0;
    //int now=chessboard->fliped;
    bool flag;
    const int pre_count=AI_check_danger(temp,now);
    for(int i=1;i<=9;++i)// same i direction
        for(int j=1;j<=9;++j) {//1 2
            if(In_chessboard(i,j) && temp.chess[i][j]==now) {
                for(int move_t=1;move_t<=3;++move_t) {
                    //if(j+move_t-1>9) continue;
                    flag=1;
                    tmp=temp;
                    tmp.tot=move_t;
                    tmp.direction=1;
                    for(int k=1;k<=move_t;++k) {
                        tmp.poi_move[k].x=i+(k-1)*v1[tmp.direction];
                        tmp.poi_move[k].y=j+(k-1)*v2[tmp.direction];
                        if(!In_chessboard(tmp.poi_move[k].x,tmp.poi_move[k].y))
                            flag=0;
                    }
                    if(flag && tmp.input()) {
                        printf("T: %d ",Total);
                        ai[++Total].tot = move_t;
                        ai[Total].direction = tmp.direction;
                        for(int k=1;k<=move_t;++k)
                            ai[Total].try_move[k]=tmp.poi_move[k];
                        tmp.Tcalcu();
                        ai[Total].value=move_t;
                        int danger_now=AI_check_danger(tmp,now);
                        int now_count=pre_count - danger_now;
                        ai[Total].value+=now_count * 100;
                        if(danger_now==0) ai[Total].value+=800;
                        if(tmp.Tblackout > black_out) ai[Total].value+=50000;
                        ai[Total].value+=AI_check_danger(tmp,3-now)*500;
                    }
                    //----------------------------------------------------------------------------------
                    flag=1;
                    tmp=temp;
                    tmp.tot=move_t;
                    tmp.direction=2;
                    for(int k=1;k<=move_t;++k) {
                        tmp.poi_move[k].x=i+(k-1)*v1[tmp.direction];
                        tmp.poi_move[k].y=j+(k-1)*v2[tmp.direction];
                        if(!In_chessboard(tmp.poi_move[k].x,tmp.poi_move[k].y))
                            flag=0;
                    }
                    if(flag && tmp.input()) {
                        ai[++Total].tot = move_t;
                        ai[Total].direction = tmp.direction;
                        for(int k=1;k<=move_t;++k)
                            ai[Total].try_move[k]=tmp.poi_move[k];
                        tmp.Tcalcu();
                        ai[Total].value=move_t;
                        int danger_now=AI_check_danger(tmp,now);
                        int now_count=pre_count - danger_now;
                        ai[Total].value+=now_count * 100;
                        if(danger_now==0) ai[Total].value+=800;
                        if(tmp.Tblackout > black_out) ai[Total].value+=50000;
                        ai[Total].value+=AI_check_danger(tmp,3-now)*500;
                    }
                }
            }
        }
    for(int j=1;j<=9;++j)// same j
        for(int i=1;i<=9;++i) {//3 4
            if(In_chessboard(i,j) && temp.chess[i][j]==now) {
                for(int move_t=1;move_t<=3;++move_t) {
                    //if(i+move_t-1>9) continue;
                    flag=1;
                    tmp=temp;
                    tmp.tot=move_t;
                    tmp.direction=3;
                    for(int k=1;k<=move_t;++k) {
                        tmp.poi_move[k].x=i+(k-1)*v1[tmp.direction];
                        tmp.poi_move[k].y=j+(k-1)*v2[tmp.direction];
                        if(!In_chessboard(tmp.poi_move[k].x,tmp.poi_move[k].y))
                            flag=0;
                    }
                    if(flag && tmp.input()) {
                        ai[++Total].tot = move_t;
                        ai[Total].direction = tmp.direction;
                        for(int k=1;k<=move_t;++k)
                            ai[Total].try_move[k]=tmp.poi_move[k];
                        tmp.Tcalcu();
                        ai[Total].value=move_t;
                        int danger_now=AI_check_danger(tmp,now);
                        int now_count=pre_count - danger_now;
                        ai[Total].value+=now_count * 100;
                        if(danger_now==0) ai[Total].value+=800;
                        if(tmp.Tblackout > black_out) ai[Total].value+=50000;
                        ai[Total].value+=AI_check_danger(tmp,3-now)*500;
                    }
                    //----------------------------------------------------------------------------------
                    flag=1;
                    tmp=temp;
                    tmp.tot=move_t;
                    tmp.direction=4;
                    for(int k=1;k<=move_t;++k) {
                        tmp.poi_move[k].x=i+(k-1)*v1[tmp.direction];
                        tmp.poi_move[k].y=j+(k-1)*v2[tmp.direction];
                        if(!In_chessboard(tmp.poi_move[k].x,tmp.poi_move[k].y))
                            flag=0;
                    }
                    if(flag && tmp.input()) {
                        ai[++Total].tot = move_t;
                        ai[Total].direction = tmp.direction;
                        for(int k=1;k<=move_t;++k)
                            ai[Total].try_move[k]=tmp.poi_move[k];
                        tmp.Tcalcu();
                        ai[Total].value=move_t;
                        int danger_now=AI_check_danger(tmp,now);
                        int now_count=pre_count - danger_now;
                        ai[Total].value+=now_count * 100;
                        if(danger_now==0) ai[Total].value+=800;
                        if(tmp.Tblackout > black_out) ai[Total].value+=50000;
                        ai[Total].value+=AI_check_danger(tmp,3-now)*500;
                    }
                }
            }
        }
    for(int sum=6;sum<=14;++sum)//same sum
        for(int i=1;i<=9;++i) {// 5 6
            int j=sum-i;
            if(In_chessboard(i,j) && temp.chess[i][j]==now) {
                for(int move_t=1;move_t<=3;++move_t) {
                    //if(j+move_t-1>9) continue;///-------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    flag=1;
                    tmp=temp;
                    tmp.tot=move_t;
                    tmp.direction=5;
                    for(int k=1;k<=move_t;++k) {
                        tmp.poi_move[k].x=i+(k-1)*v1[tmp.direction];
                        tmp.poi_move[k].y=j+(k-1)*v2[tmp.direction];
                        if(!In_chessboard(tmp.poi_move[k].x,tmp.poi_move[k].y))
                            flag=0;
                    }
                    if(flag && tmp.input()) {
                        ai[++Total].tot = move_t;
                        ai[Total].direction = tmp.direction;
                        for(int k=1;k<=move_t;++k)
                            ai[Total].try_move[k]=tmp.poi_move[k];
                        tmp.Tcalcu();
                        ai[Total].value=move_t;
                        int danger_now=AI_check_danger(tmp,now);
                        int now_count=pre_count - danger_now;
                        ai[Total].value+=now_count * 100;
                        if(danger_now==0) ai[Total].value+=800;
                        if(tmp.Tblackout > black_out) ai[Total].value+=50000;
                        ai[Total].value+=AI_check_danger(tmp,3-now)*500;
                    }
                    //----------------------------------------------------------------------------------
                    flag=1;
                    tmp=temp;
                    tmp.tot=move_t;
                    tmp.direction=6;
                    for(int k=1;k<=move_t;++k) {
                        tmp.poi_move[k].x=i+(k-1)*v1[tmp.direction];
                        tmp.poi_move[k].y=j+(k-1)*v2[tmp.direction];
                        if(!In_chessboard(tmp.poi_move[k].x,tmp.poi_move[k].y))
                            flag=0;
                    }
                    if(flag && tmp.input()) {
                        ai[++Total].tot = move_t;
                        ai[Total].direction = tmp.direction;
                        for(int k=1;k<=move_t;++k)
                            ai[Total].try_move[k]=tmp.poi_move[k];
                        tmp.Tcalcu();
                        ai[Total].value=move_t;
                        int danger_now=AI_check_danger(tmp,now);
                        int now_count=pre_count - danger_now;
                        ai[Total].value+=now_count * 100;
                        if(danger_now==0) ai[Total].value+=800;
                        if(tmp.Tblackout > black_out) ai[Total].value+=50000;
                        ai[Total].value+=AI_check_danger(tmp,3-now)*500;
                    }
                }
            }
        }
}

int MainWindow::AI_random_input(){
    srand(time(0));
    AI_trymove(chessboard->fliped);
    //printf("OK_Total %d\n",Total);
    std::sort(ai+1,ai+1+Total);
    ai[Total+1].value=-1000000;
    int tt=rand()%5;
    for(int i=2;i<=Total;++i)
    if(ai[i].value < ai[1].value-tt) {
        Total=i-1;
        break;
    }
    int x=rand()%Total+1;
    printf("Total : %d value_max : %d value : %d\n",Total,ai[1].value,ai[x].value);
    memcpy(chessboard->poi_move,ai[x].try_move,sizeof (chessboard->poi_move));
    chessboard->chess_move_count=ai[x].tot;
    return ai[x].direction;
}

void MainWindow::AI_move(){
    srand(time(0));
    //int count=1;
    //
    //    if(chessboard->gametype!=0) printf("AI_move 0\n");
    //
    Sleep(400);
    while(1){
        if(doMove(AI_random_input())){
            //printf("AI MOVED!");
            return;
        }
    }
    return;
}

bool MainWindow::doMove(int direction){
    //
        if(chessboard->gametype!=0) printf("domove 0\n");
    //
    if(chessboard->chess_move_count!=1 && chessboard->chess_move_count!=2 && chessboard->chess_move_count!=3) return 0;
    for(int i=1;i<=chessboard->chess_move_count;++i) {
        int x=chessboard->poi_move[i].x+v1[direction],y=chessboard->poi_move[i].y+v2[direction];
        if(x+y<6 || x+y>14 || x<1 || x>9 || y<1 || y>9) return 0;
    }
            for(int i=1;i<=chessboard->chess_move_count;++i)
                if(chessboard->chess[chessboard->poi_move[i].x][chessboard->poi_move[i].y]!=chessboard->fliped) return 0;
            if(chessboard->chess_move_count==1) {
                int x_new=chessboard->poi_move[1].x+v1[direction],y_new=chessboard->poi_move[1].y+v2[direction];
                if(chessboard->chess[x_new][y_new]!=0) return 0;
                else {
                    work(direction);
                    return 1;
                }
            }
            std::sort(chessboard->poi_move+1,chessboard->poi_move+1+chessboard->chess_move_count);
            if(chessboard->chess_move_count==2) {
                int direction_near=0;
                for(int i=1;i<=6;++i)
                    if(chessboard->poi_move[1].x+v1[i]==chessboard->poi_move[2].x && chessboard->poi_move[1].y+v2[i]==chessboard->poi_move[2].y) {
                        direction_near=i;
                        break;
                    }
                if(direction_near==0) return 0;
                if(direction_near&1) {
                    if(direction-1==direction_near) {
                        swap(chessboard->poi_move[1],chessboard->poi_move[2]);
                        direction_near=direction;
                        //check
                        if(chessboard->poi_move[1].x+v1[direction]!=chessboard->poi_move[2].x ||
                            chessboard->poi_move[1].y+v2[direction]!=chessboard->poi_move[2].y) {
                                puts("other error direction opposite 1");
                                for(;;);
                            }
                    }
                }
                else {//oven  near || near-1
                    if(direction+1==direction_near) {
                        swap(chessboard->poi_move[1],chessboard->poi_move[2]);
                        direction_near=direction;
                        if(chessboard->poi_move[1].x+v1[direction]!=chessboard->poi_move[2].x ||
                            chessboard->poi_move[1].y+v2[direction]!=chessboard->poi_move[2].y) {
                                puts("other error direction opposite 2");
                                for(;;);
                            }
                    }
                }
                if(direction==direction_near) {
                    int x_new=chessboard->poi_move[2].x+v1[direction],y_new=chessboard->poi_move[2].y+v2[direction];
                    int push_siz=0;
                    while(chessboard->chess[x_new][y_new]!=0) {
                        if(chessboard->chess[x_new][y_new]==chessboard->fliped) return 0;
                        else if(chessboard->chess[x_new][y_new]==3-chessboard->fliped) ++push_siz;
                        else {
                            puts("other error 01");
                            for(;;);
                        }
                        if(push_siz>1) return 0;
                        chessboard->poi_move[++chessboard->chess_move_count].x=x_new;
                        chessboard->poi_move[chessboard->chess_move_count].y=y_new;
                        x_new+=v1[direction]; y_new+=v2[direction];
                    }
                    work(direction);
                    return 1;

                }
                else {
                    bool flag=1;
                    for(int i=1;i<=chessboard->chess_move_count;++i) {
                        int x_new=chessboard->poi_move[i].x+v1[direction],y_new=chessboard->poi_move[i].y+v2[direction];
                        if(chessboard->chess[x_new][y_new]!=0) flag=0;
                    }
                    if(flag) work(direction);
                    return flag;
                }
            }
            if(chessboard->chess_move_count==3) {
                int direction_near=0;
                for(int i=1;i<=6;++i)
                    if(chessboard->poi_move[1].x+v1[i]==chessboard->poi_move[2].x && chessboard->poi_move[1].y+v2[i]==chessboard->poi_move[2].y
                        && chessboard->poi_move[2].x+v1[i]==chessboard->poi_move[3].x && chessboard->poi_move[2].y+v2[i]==chessboard->poi_move[3].y) {
                        direction_near=i;
                        break;
                    }
                if(direction_near==0) return 0;
                if(direction_near&1) {//odd near || near+1
                    if(direction-1==direction_near) {
                        swap(chessboard->poi_move[1],chessboard->poi_move[3]);
                        direction_near=direction;
                        //check
                        if(chessboard->poi_move[1].x+v1[direction]!=chessboard->poi_move[2].x ||
                            chessboard->poi_move[1].y+v2[direction]!=chessboard->poi_move[2].y ||
                            chessboard->poi_move[2].x+v1[direction]!=chessboard->poi_move[3].x ||
                            chessboard->poi_move[2].y+v2[direction]!=chessboard->poi_move[3].y) {
                                puts("other error direction opposite 3");
                                for(;;);
                            }
                    }
                }
                else {//oven  near || near-1
                    if(direction+1==direction_near) {
                        swap(chessboard->poi_move[1],chessboard->poi_move[3]);
                        direction_near=direction;
                        if(chessboard->poi_move[1].x+v1[direction]!=chessboard->poi_move[2].x ||
                            chessboard->poi_move[1].y+v2[direction]!=chessboard->poi_move[2].y ||
                            chessboard->poi_move[2].x+v1[direction]!=chessboard->poi_move[3].x ||
                            chessboard->poi_move[2].y+v2[direction]!=chessboard->poi_move[3].y) {
                                puts("other error direction opposite 3");
                                for(;;);
                            }
                    }
                }
                if(direction==direction_near) {
                    int x_new=chessboard->poi_move[3].x+v1[direction],y_new=chessboard->poi_move[3].y+v2[direction];
                    int push_siz=0;
                    while(chessboard->chess[x_new][y_new]!=0) {
                        if(chessboard->chess[x_new][y_new]==chessboard->fliped) return 0;
                        else if(chessboard->chess[x_new][y_new]==3-chessboard->fliped) ++push_siz;
                        else {
                            puts("other error 03");
                            for(;;);
                        }
                        if(push_siz>2) return 0;
                        chessboard->poi_move[++chessboard->chess_move_count].x=x_new;
                        chessboard->poi_move[chessboard->chess_move_count].y=y_new;
                        x_new+=v1[direction]; y_new+=v2[direction];
                    }
                    work(direction);
                    return 1;

                }
                else {
                    bool flag=1;
                    for(int i=1;i<=chessboard->chess_move_count;++i) {
                        int x_new=chessboard->poi_move[i].x+v1[direction],y_new=chessboard->poi_move[i].y+v2[direction];
                        if(chessboard->chess[x_new][y_new]!=0) flag=0;
                    }
                    if(flag) work(direction);
                    return flag;
                }
            }
            //bug
            puts("other error 02");
            for(;;);
}



void MainWindow::on_retract_clicked()
{
    //retract();
}

void MainWindow::on_restart_clicked()
{
    startgame();
}

void MainWindow::on_PvP_clicked()
{
    chessboard->gametype=1;
}

void MainWindow::on_PvE_clicked()
{
    chessboard->gametype=0;
}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    double p=static_cast<double>(value)/static_cast<double>(ui->verticalScrollBar->maximum());
    chessboard->move(0,-(chessboard->height()-350)*p);
    ui->centralWidget->move(0,-(ui->centralWidget->height()-350)*p);
}
