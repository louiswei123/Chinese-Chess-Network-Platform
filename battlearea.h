#ifndef BATTLEAREA_H
#define BATTLEAREA_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <chess.h>
#include <QPainter>
#include <QSound>
#include <QMediaPlayer>
#include <musicthread.h>
namespace Ui {
class BattleArea;
}


class BattleArea : public QWidget
{
    Q_OBJECT

public:
    explicit BattleArea(int color,QWidget *parent = 0);
    void getString(QString tem);
    int mycolor;            //服务器端设置为红色，客户端设置为黑色  //如果是本机那么设置为0
    double time;
    //vector<Chess*> chesspiece;
    struct
    {
        Thetype type;
        int color;   //color 0 stands for nothing
    }ChessBoard[9][10];
            //坐标映射分别是x和y
    /*
     * 棋盘上储存的是落子信息，chesspiece的vector里面储存的是每个的具体信息
     */

    ~BattleArea();
signals:
    void SendMessage(QString);
    void move(int);               //这里的信号让moveto自己发射播放信号
    //0代表没有吃子，1代表移动 2代表将军 3代表吃将 4代表游戏结束（本地模式）5代表失败 6代表胜利 7代表新消息 8不合法 9新消息
    void time_changed();
    void gameover();
protected slots:

    void on_pushButton_clicked();
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool check_if_move_legal(int xi,int yi);
    bool check_if_checkmate(int xi,int yi);
    void moveto(int xi,int yi);     //通过返回值方便日后播放音效
                                   //move to兼备检查胜利的能力
public slots:
    void win(int i);
    void surrender();
    void playaudio(int i);
    void savegame();
    void loadgame(QString in);
    void setinfo(QString tem);

private:
     QMediaPlayer musicplayer;
     MusicThread musicthread;
     Ui::BattleArea *ui;
     QTimer* timer;

     int hmargin;
     int vmargin;
     int length;   //length of each cube
     int current_color;
     int move_x;
     int move_y;
     int click_x;
     int click_y;  //relative
     int select_xi;
     int select_yi;
    // bool myturn;   //只有我自己回合的时候才能走
     int to_xi;
     int to_yi;
     int selectormove;   //0 for view,1 for select,2 for move, 3 for moveerror


};


#endif // BATTLEAREA_H
