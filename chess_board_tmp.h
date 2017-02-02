#ifndef CHESS_BOARD_TMP_H
#define CHESS_BOARD_TMP_H
#include <commdef.h>
#include <algorithm>

class chess_board_tmp
{
public:
    chess_board_tmp();
    int chess[20][20],Twhiteout,Tblackout,move,win;
    int tot,direction;//
    Point_tomove poi_move[10];//
    void Tcalcu();
    void TWork();
    bool input();
    void swap(Point_tomove &a,Point_tomove &b);
    void swap(int &a,int &b);
};

#endif // CHESS_BOARD_TMP_H
