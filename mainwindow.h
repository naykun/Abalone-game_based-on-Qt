#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <chessboard.h>
#include <chess_board_tmp.h>
#include <ctime>
#include <QMessageBox>
#include <commdef.h>
#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initActions();
    void swap(Point_tomove &a,Point_tomove &b);
    void swap(int &a,int &b);
    void work(int direction);
    void calculate();
    void startgame();
    void assign(chess_board_tmp &a);
    void AI_move();
    int AI_random_input();
    bool AI_doMove(int direction);
    void AI_trymove(int now);
    int AI_check_danger(const chess_board_tmp &tmp,int flipped);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    chessBoard *chessboard;
    int win;
    int black_out,black_out_laststate;
    int white_out,white_out_laststate;
    int chess_laststate[20][20];
    int lastfliped;
    int Total;
    struct can_move_chess {
        int tot,direction,value;
        Point_tomove try_move[10];
        bool operator <(const can_move_chess &a)const {
            return value>a.value;
        }
    } ai[1000];
protected:

private slots:
    bool doMove(int direction);
    void retract();
    void on_retract_clicked();
    void on_restart_clicked();
    void on_PvP_clicked();
    void on_PvE_clicked();
    void on_verticalScrollBar_valueChanged(int value);
};

#endif // MAINWINDOW_H
