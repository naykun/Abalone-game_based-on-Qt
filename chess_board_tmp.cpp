#include "chess_board_tmp.h"

chess_board_tmp::chess_board_tmp()
{

}
void chess_board_tmp::Tcalcu() {
        int tmp[20][20];
        memset(tmp,0,sizeof(tmp));
        int black_num=0,white_num=0;
        for(int i=1;i<=9;++i)
        for(int j=1;j<=9;++j)
        if(i+j>=6 && i+j<=14) {
            if(chess[i][j]==1) ++black_num;
            if(chess[i][j]==2) ++white_num;
            tmp[i][j]=chess[i][j];
        }
        memcpy(chess,tmp,sizeof(tmp));
        Twhiteout=14-white_num;
        Tblackout=14-black_num;
        if(Twhiteout>=6) win=1;
        if(Tblackout>=6) win=2;
        return;
}
void chess_board_tmp::TWork() {//move
        for(int i=tot;i>0;--i) {
            int x=poi_move[i].x,y=poi_move[i].y;
            swap(chess[x][y],chess[x+v1[direction]][y+v2[direction]]);
        }
}
bool chess_board_tmp::input() {//attention
        //type=0;//
        if(tot!=1 && tot!=2 && tot!=3) return 0;
        for(int i=1;i<=tot;++i) {
            int x=poi_move[i].x+v1[direction],y=poi_move[i].y+v2[direction];
            if(x+y<6 || x+y>14 || x<1 || x>9 || y<1 || y>9) return 0;
        }
        for(int i=1;i<=tot;++i) {
            int x=poi_move[i].x+v1[direction],y=poi_move[i].y+v2[direction];
            if(x+y<6 || x+y>14 || x>9 || x<1 || y>9 || y<1) return 0;
        }
        for(int i=1;i<=tot;++i)
            if(chess[poi_move[i].x][poi_move[i].y]!=move) return 0;
        if(tot==1) {
            int x_new=poi_move[1].x+v1[direction],y_new=poi_move[1].y+v2[direction];
            if(chess[x_new][y_new]!=0) return 0;
            else {
                TWork();
                return 1;
            }
        }
//direrction
        std::sort(poi_move+1,poi_move+1+tot);
        if(tot==2) {
            int direction_near=0;//
            for(int i=1;i<=6;++i)
                if(poi_move[1].x+v1[i]==poi_move[2].x && poi_move[1].y+v2[i]==poi_move[2].y) {
                    direction_near=i;
                    break;
                }
            if(direction_near==0) return 0;//
            if(direction_near&1) {//odd near || near+1
                if(direction-1==direction_near) { //
                    swap(poi_move[1],poi_move[2]);
                    direction_near=direction;
                    //check
                    if(poi_move[1].x+v1[direction]!=poi_move[2].x ||
                        poi_move[1].y+v2[direction]!=poi_move[2].y) {
                            puts("other error direction opposite 1");
                        }
                }
            }
            else {//oven  near || near-1
                if(direction+1==direction_near) {//
                    swap(poi_move[1],poi_move[2]);
                    direction_near=direction;
                    if(poi_move[1].x+v1[direction]!=poi_move[2].x ||
                        poi_move[1].y+v2[direction]!=poi_move[2].y) {
                            puts("other error direction opposite 2");
                        }
                }
            }
            if(direction==direction_near) {//
                int x_new=poi_move[2].x+v1[direction],y_new=poi_move[2].y+v2[direction];
                int push_siz=0;
                while(chess[x_new][y_new]!=0) {//
                    if(chess[x_new][y_new]==move) return 0;
                    else if(chess[x_new][y_new]==3-move) ++push_siz;
                    else {
                        puts("other error 01");//
                    }
                    if(push_siz>1) return 0;
                    poi_move[++tot].x=x_new;
                    poi_move[tot].y=y_new;
                    x_new+=v1[direction]; y_new+=v2[direction];
                }
                TWork();
                return 1;
                //
            }
            else {//
                bool flag=1;
                for(int i=1;i<=tot;++i) {
                    int x_new=poi_move[i].x+v1[direction],y_new=poi_move[i].y+v2[direction];
                    if(chess[x_new][y_new]!=0) flag=0;
                }
                if(flag) TWork();
                return flag;
            }
        }
        if(tot==3) {
            int direction_near=0;
            for(int i=1;i<=6;++i)
                if(poi_move[1].x+v1[i]==poi_move[2].x && poi_move[1].y+v2[i]==poi_move[2].y
                    && poi_move[2].x+v1[i]==poi_move[3].x && poi_move[2].y+v2[i]==poi_move[3].y) {
                    direction_near=i;
                    break;
                }
            if(direction_near==0) return 0;//
            if(direction_near&1) {//odd near || near+1
                if(direction-1==direction_near) { //
                    swap(poi_move[1],poi_move[3]);
                    direction_near=direction;
                    //check
                    if(poi_move[1].x+v1[direction]!=poi_move[2].x ||
                        poi_move[1].y+v2[direction]!=poi_move[2].y ||
                        poi_move[2].x+v1[direction]!=poi_move[3].x ||
                        poi_move[2].y+v2[direction]!=poi_move[3].y) {
                            puts("other error direction opposite 3");
                        }
                }
            }
            else {//oven  near || near-1
                if(direction+1==direction_near) {//
                    swap(poi_move[1],poi_move[3]);
                    direction_near=direction;
                    if(poi_move[1].x+v1[direction]!=poi_move[2].x ||
                        poi_move[1].y+v2[direction]!=poi_move[2].y ||
                        poi_move[2].x+v1[direction]!=poi_move[3].x ||
                        poi_move[2].y+v2[direction]!=poi_move[3].y) {
                            puts("other error direction opposite 3");
                        }
                }
            }
            if(direction==direction_near) {
                int x_new=poi_move[3].x+v1[direction],y_new=poi_move[3].y+v2[direction];
                int push_siz=0;
                while(chess[x_new][y_new]!=0) {//
                    if(chess[x_new][y_new]==move) return 0;
                    else if(chess[x_new][y_new]==3-move) ++push_siz;
                    else {
                        puts("other error 03");//
                    }
                    if(push_siz>2) return 0;
                    poi_move[++tot].x=x_new;
                    poi_move[tot].y=y_new;
                    x_new+=v1[direction]; y_new+=v2[direction];
                }
                TWork();
                return 1;
                //
            }
            else {//
                bool flag=1;
                for(int i=1;i<=tot;++i) {
                    int x_new=poi_move[i].x+v1[direction],y_new=poi_move[i].y+v2[direction];
                    if(chess[x_new][y_new]!=0) flag=0;
                }
                if(flag) TWork();
                return flag;
            }
        }
        //bug
        puts("other error 02");//
        return 0;
}
void chess_board_tmp::swap(Point_tomove &a,Point_tomove &b){
    Point_tomove t=a; a=b; b=t;
}

void chess_board_tmp::swap(int &a, int &b){
    int t=a; a=b; b=t;
}
