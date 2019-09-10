#include "battlearea.h"
#include "ui_battlearea.h"
#include <QDebug>
#include <math.h>
#include <QTimer>
#include <QMessageBox>
#include <mainwindow.h>
#include <mediafile.h>



BattleArea::BattleArea(int color,QWidget *parent) :
    QWidget(parent),mycolor(color),
    ui(new Ui::BattleArea)
{
    ui->setupUi(this);
    musicthread.run();
    this->setAttribute(Qt::WA_DeleteOnClose);
    selectormove=0;

    this->setMouseTracking(1);
    connect(this,&BattleArea::move,this,&BattleArea::playaudio);
    for(int i=0;i<9;i++)
        for(int j=0;j<10;j++)
        {
            ChessBoard[i][j].color=0;
            ChessBoard[i][j].type=none;
        }

    if(mycolor!=0)
    {
        ChessBoard[0][0].type=ju;   ChessBoard[0][0].color=3-mycolor;
        ChessBoard[1][0].type=ma;   ChessBoard[1][0].color=3-mycolor;
        ChessBoard[2][0].type=xiang;ChessBoard[2][0].color=3-mycolor;
        ChessBoard[3][0].type=shi;  ChessBoard[3][0].color=3-mycolor;
        ChessBoard[4][0].type=jiang;
        ChessBoard[5][0].type=shi;
        ChessBoard[6][0].type=xiang;
        ChessBoard[7][0].type=ma;
        ChessBoard[8][0].type=ju;
        ChessBoard[1][2].type=pao;
        ChessBoard[7][2].type=pao;
        for(int i=0;i<=8;i+=2)
        {ChessBoard[i][3].type=bing;ChessBoard[i][3].color=3-mycolor;}
        for(int i=0;i<=8;i++)
            ChessBoard[i][0].color=3-mycolor;
        ChessBoard[1][2].color=3-mycolor;
        ChessBoard[7][2].color=3-mycolor;

        ChessBoard[0][9].type=ju;
        ChessBoard[1][9].type=ma;
        ChessBoard[2][9].type=xiang;
        ChessBoard[3][9].type=shi;
        ChessBoard[4][9].type=jiang;
        ChessBoard[5][9].type=shi;
        ChessBoard[6][9].type=xiang;
        ChessBoard[7][9].type=ma;
        ChessBoard[8][9].type=ju;
        ChessBoard[1][7].type=pao;
        ChessBoard[7][7].type=pao;
        for(int i=0;i<=8;i+=2)
        {ChessBoard[i][6].type=bing;ChessBoard[i][6].color=mycolor;}
        for(int i=0;i<=8;i++)
            ChessBoard[i][9].color=mycolor;
        ChessBoard[1][7].color=mycolor;
        ChessBoard[7][7].color=mycolor;

    }
    if(mycolor==0)
    {
        ChessBoard[0][0].type=ju;
        ChessBoard[1][0].type=ma;
        ChessBoard[2][0].type=xiang;
        ChessBoard[3][0].type=shi;
        ChessBoard[4][0].type=jiang;
        ChessBoard[5][0].type=shi;
        ChessBoard[6][0].type=xiang;
        ChessBoard[7][0].type=ma;
        ChessBoard[8][0].type=ju;
        ChessBoard[1][2].type=pao;
        ChessBoard[7][2].type=pao;
        for(int i=0;i<=8;i+=2)
        {ChessBoard[i][3].type=bing;ChessBoard[i][3].color=2;}
        for(int i=0;i<=8;i++)
            ChessBoard[i][0].color=2;
        ChessBoard[1][2].color=2;
        ChessBoard[7][2].color=2;

        ChessBoard[0][9].type=ju;
        ChessBoard[1][9].type=ma;
        ChessBoard[2][9].type=xiang;
        ChessBoard[3][9].type=shi;
        ChessBoard[4][9].type=jiang;
        ChessBoard[5][9].type=shi;
        ChessBoard[6][9].type=xiang;
        ChessBoard[7][9].type=ma;
        ChessBoard[8][9].type=ju;
        ChessBoard[1][7].type=pao;
        ChessBoard[7][7].type=pao;
        for(int i=0;i<=8;i+=2)
        {ChessBoard[i][6].type=bing;ChessBoard[i][6].color=1;}
        for(int i=0;i<=8;i++)
            ChessBoard[i][9].color=1;
        ChessBoard[1][7].color=1;
        ChessBoard[7][7].color=1;
    }

    current_color=1;
    ui->pushButton->setFocusPolicy(Qt::StrongFocus);

    time=20;
    timer=new QTimer(this);
    timer->setInterval(10);
    timer->setSingleShot(false);
    connect(timer,&QTimer::timeout,
            [&]{
                        time=time-0.01;
                        ui->lcdNumber->display(time);
                        if(mycolor==current_color&&time<=0)
                        {surrender();timer->stop();}
                        else
                        {
                           if(time<=0)
                               timer->stop();

                        }
               });


        timer->start();







}

void BattleArea:: getString(QString tem)
{
    if(tem[0]=='3')
    {
        tem.remove(0,1);
        emit move(9);
        tem="您的对手: "+tem;
        ui->plainTextEdit->appendPlainText(tem);
        emit move(7);
    }
    if(tem[0]=='1')
    {
        QMessageBox msgBox;
        if(current_color!=mycolor)
        {
            msgBox.setText("He has surrendered！！！");
            ui->plainTextEdit->appendPlainText("He has surrendered!!!");
            emit move(6);
        }

    }
    if(tem[0]=='0')
    {
        if(mycolor==1)
        {
            select_xi=tem[1].digitValue();
            select_yi=9-tem[2].digitValue();
            int xi=tem[3].digitValue();
            int yi=9-tem[4].digitValue();
            moveto(xi,yi);
        }
        else
        {
            select_xi=8-tem[1].digitValue();
            select_yi=tem[2].digitValue();
            int xi=8-tem[3].digitValue();
            int yi=  tem[4].digitValue();
            moveto(xi,yi);
        }
    }

}

/*只有操控权在自己手里的时候才可以投降*/
void BattleArea::surrender()
{
    if(mycolor==0)
        {
            QMessageBox msgBox;
            if(current_color==1)
            {msgBox.setText("Red has surrendered！！！");
             ui->plainTextEdit->appendPlainText("Red has surrendered!!!");
            }
            if(current_color==2)
            {msgBox.setText("BLack has surrendered！！！");
             ui->plainTextEdit->appendPlainText("Black has surrendered!!!");
            }
            emit move(4);
            emit gameover();
            msgBox.exec();
        }
    if(mycolor!=0&&mycolor==current_color)
        {
                QMessageBox msgBox;
                emit SendMessage("10000");
                msgBox.setText("You have surrendered！！！");
                ui->plainTextEdit->appendPlainText("You have surrendered!!!");
                emit move(5);
                MainWindow* p=(MainWindow*)parentWidget();
                p->disconnect(1);
                emit gameover();
            msgBox.exec();
    }

}

BattleArea::~BattleArea()
{
    delete ui;
    delete timer;
}
void BattleArea::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::EnterKeyDefault)
    {
        QString tem=ui->lineEdit->text();
        if(!tem.isEmpty())
            {
            ui->plainTextEdit->appendPlainText("Me: "+ui->lineEdit->text());
            ui->lineEdit->clear();
            tem.insert(0,'3');emit SendMessage(tem);
            }



    }
}
void BattleArea::on_pushButton_clicked()  //sendMessage
{
    QString tem=ui->lineEdit->text();
    if(!tem.isEmpty())
        {
        ui->plainTextEdit->appendPlainText("Me: "+ui->lineEdit->text());
        ui->lineEdit->clear();
        tem.insert(0,'3');emit SendMessage(tem);
        }

}

void BattleArea::win(int i)
{

    QMessageBox msgBox;
    if(i==1)
        msgBox.setText("Red won!!!");
    else
        msgBox.setText("Black won!!!");
    if(i!=current_color)
    {emit gameover();qDebug()<<"lose";}

    msgBox.exec();

}


void BattleArea::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
            opt.init(this);
            QPainter p(this);
            style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    //初始化的时候一定要放在paintevent里面
    length=height()/9-5;
    vmargin=(height()-length*9)/2;
    hmargin= ((width()-ui->plainTextEdit->width())-8*length)/2;
    QPainter* painter=new QPainter;
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing, true); // 抗锯齿
    painter->setPen(QPen(Qt::black,4,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->drawRect(hmargin-4,vmargin-4,8*length+8,9*length+8);

    painter->setPen(QPen(Qt::black,2,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin));
    painter->drawRect(hmargin,vmargin,8*length,9*length);
    //画横线
    for(int i=1;i<=8;i++)
        painter->drawLine(hmargin,vmargin+i*length,hmargin+8*length,vmargin+i*length);
    //画竖线
    for(int i=1;i<=7;i++)
    {
        painter->drawLine(hmargin+i*length,vmargin,hmargin+i*length,vmargin+4*length);
        painter->drawLine(hmargin+i*length,vmargin+5*length,hmargin+i*length,vmargin+9*length);
    }
    //画斜线
    painter->drawLine(hmargin+3*length,vmargin,hmargin+5*length,vmargin+length*2);
    painter->drawLine(hmargin+3*length,vmargin+2*length,hmargin+5*length,vmargin);
    painter->drawLine(hmargin+3*length,vmargin+9*length,hmargin+5*length,vmargin+length*7);
    painter->drawLine(hmargin+3*length,vmargin+7*length,hmargin+5*length,vmargin+length*9);
    //画兵炮位置的装饰线




        for(int j=3;j<=6;j+=3)
            for(int i=0;i<=8;i+=2)
            {
                if(i!=0)
                {
                    //先画左边的
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j-4,hmargin+length*i-4,vmargin+length*j-8);
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j-4,hmargin+length*i-8,vmargin+length*j-4);
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j+4,hmargin+length*i-4,vmargin+length*j+8);
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j+4,hmargin+length*i-8,vmargin+length*j+4);

                }
                if(i!=8)//话右边的
                {
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j-4,hmargin+8+length*i,vmargin+length*j-4);
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j-4,hmargin+4+length*i,vmargin+length*j-8);
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j+4,hmargin+4+length*i,vmargin+length*j+8);
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j+4,hmargin+8+length*i,vmargin+length*j+4);
                }
            }
        for(int j=2;j<=7;j+=5)
            for(int i=1;i<=7;i+=6)
            {
                if(i!=0)
                    {
                                    //先画左边的
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j-4,hmargin+length*i-4,vmargin+length*j-8);
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j-4,hmargin+length*i-8,vmargin+length*j-4);
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j+4,hmargin+length*i-4,vmargin+length*j+8);
                    painter->drawLine(hmargin+length*i-4,vmargin+length*j+4,hmargin+length*i-8,vmargin+length*j+4);

                    }
                    if(i!=8)//话右边的
                    {
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j-4,hmargin+8+length*i,vmargin+length*j-4);
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j-4,hmargin+4+length*i,vmargin+length*j-8);
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j+4,hmargin+4+length*i,vmargin+length*j+8);
                    painter->drawLine(hmargin+4+length*i,vmargin+length*j+4,hmargin+8+length*i,vmargin+length*j+4);
                    }
            }


    QPointF point(hmargin+length*1,vmargin+4.75*length);

        painter->setFont(QFont("Lisu", 30));
        painter->drawText(point, "楚河");
    QPointF point1(hmargin+5.6*length,vmargin+4.75*length);
        painter->drawText(point1,"漢界");


    for(int i=0;i<9;i++) //x
        for(int j=0;j<10;j++)
        {
            QRect target(hmargin+i*length-22,vmargin+j*length-22,44,44);
            QRect source(0,0,50,50);
            if(ChessBoard[i][j].color!=0)
            {
                if(ChessBoard[i][j].color==1)
                {
                     if(ChessBoard[i][j].type==bing)
                     {
                         QImage image(":/piece/icon/image/bing1.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==shi)
                     {
                         QImage image(":/piece/icon/image/shi1.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==ma)
                     {
                         QImage image(":/piece/icon/image/ma1.png");
                        painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==ju)
                     {
                         QImage image(":/piece/icon/image/ju1.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==jiang)
                     {
                         QImage image(":/piece/icon/image/jiang1.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==xiang)
                     {
                         QImage image(":/piece/icon/image/xiang1.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==pao)
                     {
                         QImage image(":/piece/icon/image/pao1.png");
                         painter->drawImage(target,image,source);
                     }

                }
                if(ChessBoard[i][j].color==2)
                {
                     if(ChessBoard[i][j].type==bing)
                     {
                         QImage image(":/piece/icon/image/bing2.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==shi)
                     {
                         QImage image(":/piece/icon/image/shi2.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==ma)
                     {
                         QImage image(":/piece/icon/image/ma2.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==ju)
                     {
                         QImage image(":/piece/icon/image/ju2.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==jiang)
                     {
                         QImage image(":/piece/icon/image/jiang2.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==xiang)
                     {
                         QImage image(":/piece/icon/image/xiang2.png");
                         painter->drawImage(target,image,source);
                     }
                     if(ChessBoard[i][j].type==pao)
                     {
                         QImage image(":/piece/icon/image/pao2.png");
                         painter->drawImage(target,image,source);
                     }


                }

            }
        }
    /*
     *表达棋盘上现在有的子
     */

    int tem_i=move_x/length;
    int tem_j=move_y/length;
    int tem_rx=move_x-tem_i*length;   //relative
    int tem_ry=move_y-tem_j*length;
    if(move_x>=0)
    {
        if(tem_rx<length/2&&tem_ry<length/2)
        {}
        else if(tem_rx<length/2&&tem_ry>length/2)
            tem_j++;
        else if(tem_rx>length/2&&tem_ry<length/2)
            tem_i++;
        else if(tem_rx>length/2&&tem_ry>length/2)
            {tem_i++;tem_j++;}

        if(selectormove==0)
            painter->setPen(QPen(Qt::lightGray,2,Qt::DashDotDotLine,Qt::SquareCap,Qt::BevelJoin));
        if(selectormove==1)
            painter->setPen(QPen(Qt::gray,2,Qt::DashDotDotLine,Qt::SquareCap,Qt::BevelJoin));
        if(selectormove==3)
            painter->setPen(QPen(Qt::red,2,Qt::DashDotDotLine,Qt::SquareCap,Qt::BevelJoin));
        /*
         * 带入一个函数判断是否可走,mousepress中判断
         */
        painter->drawRect(hmargin+(tem_i*length)-25,vmargin+(tem_j*length)-25,50,50);

        painter->setPen(QPen(Qt::cyan,2,Qt::DashDotDotLine,Qt::SquareCap,Qt::BevelJoin));
        painter->drawRect(hmargin+(select_xi*length)-25,vmargin+(select_yi*length)-25,50,50);
    }

    delete painter;



}
void BattleArea::mouseMoveEvent(QMouseEvent *event)
{
   if(event->x()>hmargin&&event->x()<hmargin+8*length&&event->y()>vmargin&&event->y()<vmargin+length*9)
   {
       move_x=event->x()-hmargin;
       move_y=event->y()-vmargin;
       update();
   }
   else
   {
       move_x=-100;
       move_y=-100;
   }

}
void BattleArea::mousePressEvent(QMouseEvent *event)
{
    if(current_color==mycolor||mycolor==0)  //如果是网络联机版的画只能由处理string的函数来改变color
    {
        if(event->x()>hmargin&&event->x()<hmargin+8*length&&event->y()>vmargin&&event->y()<vmargin+length*9)
        {
            click_x=event->x()-hmargin;
            click_y=event->y()-vmargin;
            int tem_i=click_x/length;
            int tem_j=click_y/length;
            int tem_rx=click_x-tem_i*length;   //relative
            int tem_ry=click_y-tem_j*length;
            if(move_x>=0)
            {
                if(tem_rx<length/2&&tem_ry<length/2)
                {}
                else if(tem_rx<length/2&&tem_ry>length/2)
                    tem_j++;
                else if(tem_rx>length/2&&tem_ry<length/2)
                    tem_i++;
                else if(tem_rx>length/2&&tem_ry>length/2)
                {tem_i++;tem_j++;}
            }

            if(ChessBoard[tem_i][tem_j].color==current_color||(selectormove==1)||ChessBoard[tem_i][tem_j].color==0)    //只有己方的棋子才可以被选中或空白
            {
                if(selectormove==1||selectormove==3)
                {

                    if(check_if_move_legal(tem_i,tem_j))
                    {
                        selectormove=0;
                        to_xi=tem_i;
                        to_yi=tem_j;
                        int color=current_color;

                        moveto(to_xi,to_yi);   //走棋
                        select_xi=-100;
                        select_yi=-100;
                        selectormove=0;
                        current_color=3-color;
                         update();
                         return;

                    }
                    else
                    {
                        selectormove=3;

                        update();

                        QTimer* timer=new QTimer(this);
                        emit move(8);
                        timer->singleShot(1000,[this]{selectormove=0;select_xi=-100,select_yi=-100;});
                         update();
                         return;
                    }

                    /*判断是否走是否合法*/
                }


                if(selectormove==0&&ChessBoard[tem_i][tem_j].color!=0)
                {
                    selectormove=1;
                    select_xi=tem_i;
                    select_yi=tem_j;
                     update();
                     return;
                }




            }
        }
        else
        {
            click_x=-100;
            click_y=-100;
            select_xi=-100;
            select_yi=-100;
            selectormove=0;
             update();
        }

    }

}
void BattleArea::moveto(int xi,int yi)
{
    //1代表吃子，0代表没有吃子,2代表将军？    3//代表胜利
    //不需要调用下面的check legal
    //需要判断

    vector<QString> name {"兵","炮","车","马","相","士","将"};
    vector<QString> order{"一","二","三","四","五","六","七","八","九","十"};
    vector<QString> order_2{"1","2","3","4","5","6","7","8","9","10"};
    time=20;

    //表达棋局上走子的信息
    QString info;
    if(mycolor==0||mycolor==1)
        {
            if(current_color==1)
            {
                if(yi==select_yi)
                {

                      info = name[ChessBoard[select_xi][select_yi].type]+order[select_xi]+"平"+order[xi];

                }
                else
                {
                    if(yi<select_yi)
                       info = name[ChessBoard[select_xi][select_yi].type]+order[select_xi]+"进"+order[select_yi-yi-1];
                    else
                       info = name[ChessBoard[select_xi][select_yi].type]+order[select_xi]+"退"+order[yi-select_yi-1];
                }
            }
            else
            {
                if(yi==select_yi)
                {

                      info = name[ChessBoard[select_xi][select_yi].type]+order_2[select_xi]+"平"+order_2[xi];

                }
                else
                {
                    if(yi>select_yi)
                       info = name[ChessBoard[select_xi][select_yi].type]+order_2[select_xi]+"进"+order_2[yi-select_yi-1];
                    else
                       info = name[ChessBoard[select_xi][select_yi].type]+order_2[select_xi]+"退"+order_2[select_yi-1-yi];
                }
            }
        }
    else
    {
        if(current_color==1)
        {
            if(yi==select_yi)
            {

                  info = name[ChessBoard[select_xi][select_yi].type]+order[select_xi]+"平"+order[xi];

            }
            else
            {
                if(yi<select_yi)
                   info = name[ChessBoard[select_xi][select_yi].type]+order[select_xi]+"退"+order[select_yi-yi-1];
                else
                   info = name[ChessBoard[select_xi][select_yi].type]+order[select_xi]+"进"+order[yi-select_yi-1];
            }
        }
        else
        {
            if(yi==select_yi)
            {

                  info = name[ChessBoard[select_xi][select_yi].type]+order_2[select_xi]+"平"+order_2[xi];

            }
            else
            {
                if(yi>select_yi)
                   info = name[ChessBoard[select_xi][select_yi].type]+order_2[select_xi]+"退"+order_2[yi-select_yi-1];
                else
                   info = name[ChessBoard[select_xi][select_yi].type]+order_2[select_xi]+"进"+order_2[select_yi-1-yi];
            }
        }

    }
        ui->plainTextEdit->appendPlainText(info);

    //向对方传送走子的信息
    QString move_info;
    //需要对于坐标进行转换
    if(mycolor!=0&&mycolor==current_color)
    {if(mycolor==1)
        {
            int send_selectxi=select_xi;
            int send_selectyi=9-select_yi;
            int send_xi=xi;
            int send_yi=9-yi;
            move_info='0'+QString::number(send_selectxi)+QString::number(send_selectyi)+QString::number(send_xi)+QString::number(send_yi);

                emit SendMessage(move_info);
        }
     if(mycolor==2)
        {
            int send_selectxi=8-select_xi;
            int send_selectyi=9-(9-select_yi);
            int send_xi=8-xi;
            int send_yi=yi;
            move_info='0'+QString::number(send_selectxi)+QString::number(send_selectyi)+QString::number(send_xi)+QString::number(send_yi);
            emit SendMessage(move_info);
        }


    }
    //吃将的情况


    if(ChessBoard[xi][yi].type==jiang)
    {
        if(mycolor==current_color)
            emit move(3);    //只有本人移动才可以发送信号
        else
            emit move(5);
        if(current_color==1)
        {   ChessBoard[xi][yi].type=ChessBoard[select_xi][select_yi].type;
            ChessBoard[xi][yi].color=ChessBoard[select_xi][select_yi].color;
            ChessBoard[select_xi][select_yi].color=0;
            ChessBoard[select_xi][select_yi].type=none;
            select_xi=-100;
            select_yi=-100;
            selectormove=0;
            to_xi=-100;
            to_yi=-100;
            current_color=3-current_color;
            win(1);

        }
        else
        {
            ChessBoard[xi][yi].type=ChessBoard[select_xi][select_yi].type;
            ChessBoard[xi][yi].color=ChessBoard[select_xi][select_yi].color;
            ChessBoard[select_xi][select_yi].color=0;
            ChessBoard[select_xi][select_yi].type=none;
            select_xi=-100;
            select_yi=-100;
            selectormove=0;
            to_xi=-100;
            to_yi=-100;
            current_color=3-current_color;
            win(2);



        }
        if(current_color==1)
            ui->plainTextEdit->appendPlainText("该红方走了！您可以选择重新开始一局游戏！");
        if(current_color==2)
            ui->plainTextEdit->appendPlainText("该黑方走了！您可以选择重新开始一局游戏！");
        //mycolor=0;
    return;
         //注意这里处理指针的情况？？？
    }
    /*
    if(check_if_checkmate(xi,yi))
    {
        //检查将军
        ChessBoard[xi][yi].type=ChessBoard[select_xi][select_yi].type;
        ChessBoard[xi][yi].color=ChessBoard[select_xi][select_yi].color;
        ChessBoard[select_xi][select_yi].color=0;
        ChessBoard[select_xi][select_yi].type=none;
        emit move(2);
        current_color=3-current_color;
        return 2;
    }*/

    //判断吃子还是没有吃子的情况，现在不需要有悔棋的功能
    if(ChessBoard[xi][yi].color==(3-current_color))
    {
        ChessBoard[xi][yi].type=ChessBoard[select_xi][select_yi].type;
        ChessBoard[xi][yi].color=ChessBoard[select_xi][select_yi].color;
        ChessBoard[select_xi][select_yi].color=0;
        ChessBoard[select_xi][select_yi].type=none;
        //写进Qstring
        select_xi=-100;
        select_yi=-100;
        selectormove=0;
        to_xi=-100;
        to_yi=-100;
        current_color=3-current_color;
        if(current_color==1)
            ui->plainTextEdit->appendPlainText("该红方走了！");
        if(current_color==2)
            ui->plainTextEdit->appendPlainText("该黑方走了！");
        emit move(1);
        update();


    }
    if(ChessBoard[xi][yi].color==0)
    {
        ChessBoard[xi][yi].type=ChessBoard[select_xi][select_yi].type;
        ChessBoard[xi][yi].color=ChessBoard[select_xi][select_yi].color;
        ChessBoard[select_xi][select_yi].color=0;
        ChessBoard[select_xi][select_yi].type=none;
        select_xi=-100;
        select_yi=-100;
        selectormove=0;
        to_xi=-100;
        to_yi=-100;
        current_color=3-current_color;
        if(current_color==1)
            ui->plainTextEdit->appendPlainText("该红方走了！");
        if(current_color==2)
            ui->plainTextEdit->appendPlainText("该黑方走了！");
        emit move(0);

        update();


    }

    if(check_if_checkmate(xi,yi))
        {
            //int i=check_if_checkmate(1,1);
            //ui->plainTextEdit->appendPlainText(QString::number(i));
            emit move(2);
        }



}

bool BattleArea::check_if_checkmate(int xi,int yi)
{

    int temi;int temj;int offense_color;
    for(int i=3;i<=5;i++)
        for(int j=7;j<=9;j++)
            if(ChessBoard[i][j].type==jiang)
            {
                temi=i;temj=j;
                offense_color=3-ChessBoard[i][j].color;
            }
    int count=0;  //判断照将
    for(int j=temj-1;j>=0;j--)
    {
        if(ChessBoard[temi][j].type==jiang)
            return true;
        if(ChessBoard[temi][j].color!=0)
            //count++;
        //if(count!=0)
            break;

    }


    //先判断下面的老将
    if(ChessBoard[temi+1][temj].type==bing||ChessBoard[temi][temj-1].type==bing||ChessBoard[temi-1][temj].type==bing)
         return true;
    //车
    int temi_1;
    for(temi_1=temi+1;temi_1<=8;temi_1++)
        if(ChessBoard[temi_1][temj].color!=0)
            break;
    if(ChessBoard[temi_1][temj].type==ju&&ChessBoard[temi_1][temj].color==offense_color)
        return true;
    else
        {
             temi_1++;
             for(temi_1;temi_1<=8;temi_1++)
                 if(ChessBoard[temi_1][temj].color!=0)
                     break;
             if(ChessBoard[temi_1][temj].type==pao&&ChessBoard[temi_1][temj].color==offense_color)
                 return true;
        }

    for(temi_1=temi-1;temi_1>=0;temi_1--)
        if(ChessBoard[temi_1][temj].color!=0)
            break;
    if(ChessBoard[temi_1][temj].type==ju&&ChessBoard[temi_1][temj].color==offense_color)
        return true;
    else
        {
             temi_1--;
             for(temi_1;temi_1>=0;temi_1--)
                 if(ChessBoard[temi_1][temj].color!=0)
                     break;
             if(ChessBoard[temi_1][temj].type==pao&&ChessBoard[temi_1][temj].color==offense_color)
                 return true;

        }

    int temj_1;
    for(temj_1=temj+1;temj_1<=9;temj_1++)
        if(ChessBoard[temi][temj_1].color!=0)
            break;
    if(ChessBoard[temi][temj_1].type==ju&&ChessBoard[temi][temj_1].color==offense_color)
        return true;
    else
        {
                temj_1++;
             for(temj_1;temj_1<=9;temj_1++)
                 if(ChessBoard[temi][temj_1].color!=0)
                     break;
             if(ChessBoard[temi][temj_1].type==pao&&ChessBoard[temi][temj_1].color==offense_color)
                 return true;

        }

    for(temj_1=temj-1;temj_1>=0;temj_1--)
        if(ChessBoard[temi][temj_1].color!=0)
            break;
    if(ChessBoard[temi][temj_1].type==ju&&ChessBoard[temi][temj_1].color==offense_color)
        return true;
    else
        {
             temj_1--;
             for(temj_1;temj_1>=0;temj_1--)
                 if(ChessBoard[temi][temj_1].color!=0)
                     break;
             if(ChessBoard[temi][temj_1].type==pao&&ChessBoard[temi][temj_1].color==offense_color)
                 return true;

        }
    //判断马的情况
    for(int i=1;i<=7;i++)
        for(int j=5;j<=9;j++)
            if(ChessBoard[i][j].type==ma)
                if(ChessBoard[i][j].color==offense_color)
                    if((abs(i-temi)==2&&abs(j-temj)==1)||(abs(i-temi)==1&&abs(j-temj)==2))
                        if(abs(i-temi)==2)
                            {
                                if(ChessBoard[i+(temi-i)/2][j].color==0)
                                    return true;
                            }
                        else
                            {
                                if(ChessBoard[i][(temj-j)/2+j].color==0)
                                    return true;
                            }

    //上面的老将
    for(int i=3;i<=5;i++)
        for(int j=0;j<=2;j++)
            if(ChessBoard[i][j].type==jiang)
            {
                temi=i;temj=j;
                offense_color=3-ChessBoard[i][j].color;
            }

    if(ChessBoard[temi+1][temj].type==bing||ChessBoard[temi][temj+1].type==bing||ChessBoard[temi-1][temj].type==bing)
         return true;
    //车

    for(temi_1=temi+1;temi_1<=8;temi_1++)
        if(ChessBoard[temi_1][temj].color!=0)
            break;
    if(ChessBoard[temi_1][temj].type==ju&&ChessBoard[temi_1][temj].color==offense_color)
        return true;
    else
        {
             temi_1++;
             for(temi_1;temi_1<=8;temi_1++)
                 if(ChessBoard[temi_1][temj].color!=0)
                     break;
             if(ChessBoard[temi_1][temj].type==pao&&ChessBoard[temi_1][temj].color==offense_color)
                 return true;
        }

    for(temi_1=temi-1;temi_1>=0;temi_1--)
        if(ChessBoard[temi_1][temj].color!=0)
            break;
    if(ChessBoard[temi_1][temj].type==ju&&ChessBoard[temi_1][temj].color==offense_color)
        return true;
    else
        {
             temi_1--;
             for(temi_1;temi_1>=0;temi_1--)
                 if(ChessBoard[temi_1][temj].color!=0)
                     break;
             if(ChessBoard[temi_1][temj].type==pao&&ChessBoard[temi_1][temj].color==offense_color)
                 return true;

        }


    for(temj_1=temj+1;temj_1<=9;temj_1++)
        if(ChessBoard[temi][temj_1].color!=0)
            break;
    if(ChessBoard[temi][temj_1].type==ju&&ChessBoard[temi][temj_1].color==offense_color)
        return true;
    else
        {
                temj_1++;
             for(temj_1;temj_1<=9;temj_1++)
                 if(ChessBoard[temi][temj_1].color!=0)
                     break;
             if(ChessBoard[temi][temj_1].type==pao&&ChessBoard[temi][temj_1].color==offense_color)
                 return true;

        }

    for(temj_1=temj-1;temj_1>=0;temj_1--)
        if(ChessBoard[temi][temj_1].color!=0)
            break;
    if(ChessBoard[temi][temj_1].type==ju&&ChessBoard[temi][temj_1].color==offense_color)
        return true;
    else
        {
             temj_1--;
             for(temj_1;temj_1>=0;temj_1--)
                 if(ChessBoard[temi][temj_1].color!=0)
                     break;
             if(ChessBoard[temi][temj_1].type==pao&&ChessBoard[temi][temj_1].color==offense_color)
                 return true;

        }
    //判断马的情况
    for(int i=1;i<=7;i++)
        for(int j=0;j<=5;j++)
            if(ChessBoard[i][j].type==ma)
                if(ChessBoard[i][j].color==offense_color)
                    if((abs(i-temi)==2&&abs(j-temj)==1)||(abs(i-temi)==1&&abs(j-temj)==2))
                        if(abs(i-temi)==2)
                            {
                                if(ChessBoard[i+(temi-i)/2][j].color==0)
                                    return true;
                            }
                        else
                            {
                                if(ChessBoard[i][(temj-j)/2+j].color==0)
                                    return true;
                            }







    return false;

}
bool BattleArea::check_if_move_legal(int xi,int yi)   //move to xi,yi
{
    if(select_xi<0)
        return false;
    if(select_xi==xi&&select_yi==yi)
        return false;
    if(ChessBoard[xi][yi].color==current_color)
        return false;                    //不能走到自己的棋上

    Thetype check= ChessBoard[select_xi][select_yi].type;

    if(check==bing)
    {
        if(mycolor==0)   //本机模式的走法需要考虑两个方向
            {
                if(current_color==1)
                {
                    if(select_yi>=5)
                    {
                        if(abs(xi-select_xi)+abs(yi-select_yi)>1||yi>select_yi)
                            return false;
                        if(xi!=select_xi)
                            return false;

                        return true;
                    }
                    else
                    {
                        if(abs(xi-select_xi)+abs(yi-select_yi)>1||yi>select_yi)
                            return false;
                        else
                            return true;
                    }
                }
                if(current_color==2)
                {
                   if(select_yi<=4)                                 //黑方
                   {
                        if(abs(xi-select_xi)+abs(yi-select_yi)>1||yi<select_yi)
                            return false;
                        if(xi!=select_xi)
                            return false;
                        return true;
                   }
                   else
                   {
                       if(abs(xi-select_xi)+abs(yi-select_yi)>1||yi<select_yi)
                            return false;
                        else
                            return true;
                   }

                }
            }
            else
            {
                                if(select_yi>=5)
                                {
                                    if(abs(xi-select_xi)+abs(yi-select_yi)>1||yi>select_yi)
                                        return false;
                                    if(xi!=select_xi)
                                        return false;

                                    return true;
                                }
                                else
                                {
                                    if(abs(xi-select_xi)+abs(yi-select_yi)>1||yi>select_yi)
                                        return false;
                                    else
                                        return true;
                                }
            }
        }

    if(check==ma)
    {
        if((abs(select_xi-xi)==2&&abs(select_yi-yi)==1)||(abs(select_xi-xi)==1&&abs(select_yi-yi)==2))
            {
                if(abs(select_yi-yi)==2)
                    if(ChessBoard[select_xi][select_yi+(yi-select_yi)/2].color!=0)
                        return false;
                if(abs(select_xi-xi)==2)
                    if(ChessBoard[select_xi+(xi-select_xi)/2][select_yi].color!=0)
                        return false;

            }
        else
        {
            return false;
        }
    }

    if(check==ju)
    {

        if(xi!=select_xi&&yi!=select_yi)
            return false;

        if(xi==select_xi)
        {
            int min;int max;
            if(yi<select_yi)
                {min=yi;max=select_yi;}
            else
                {min=select_yi;max=yi;}
            min++;
            for(int j=min+1;j<max;j++)
                if(ChessBoard[xi][j].color!=0)
                    return false;


        }
        if(yi==select_yi)
        {
            int min;int max;
            if(xi<select_xi)
                {min=xi;max=select_xi;}
            else
                {max=xi;min=select_xi;}
            min++;
            for(int i=min+1;i<max;i++)
                if(ChessBoard[i][yi].color!=0)
                    return false;
        }
        return true;
    }

    if(check==pao)
    {
        if(xi!=select_xi&&yi!=select_yi)
            return false;


        if(xi==select_xi)
        {
            int min;int max;
            if(yi<select_yi)
                {min=yi;max=select_yi;}
            else
                {min=select_yi;max=yi;}

            int count=0;
            for(int j=min+1;j<max;j++)
                if(ChessBoard[xi][j].color!=0)
                    {
                        count++;
                        if(count>1)
                            return false;
                    }
            if(count==1)
                if(ChessBoard[xi][yi].color==0)
                    return false;
                else
                    return true;
            if(count==0)
            {
                if(ChessBoard[xi][yi].color!=0)
                    return false;
                else
                    return true;
            }


        }
        if(yi==select_yi)
        {
            int min;int max;
            if(xi<select_xi)
                {min=xi;max=select_xi;}
            else
                {max=xi;min=select_xi;}
            int count=0;
            for(int i=min+1;i<max;i++)
                if(ChessBoard[i][yi].color!=0)
                {
                    count++;
                    if(count>1)
                        return false;
                }

                if(count==0)
                {
                    if(ChessBoard[xi][yi].color!=0)
                        return false;
                    else
                        return true;
                }
        }
        return true;

    }

    if(check==xiang)
    {

        if(mycolor==0)
        {
            if(current_color==1)
            {
                if(yi<=4)
                    return false;
                if(!(abs(yi-select_yi)==2&&abs(xi-select_xi)==2))
                    return false;
               int temxi=select_xi+(xi-select_xi)/2;
               int temyi=select_yi+(yi-select_yi)/2;
                if(ChessBoard[temxi][temyi].color!=0)
                    return false;
                return true;
            }
            if(current_color==2)
            {
                if(yi>=5)
                    return false;
                if(!(abs(yi-select_yi)==2&&abs(xi-select_xi)==2))
                    return false;
                int temxi=select_xi+(xi-select_xi)/2;
                int temyi=select_yi+(yi-select_yi)/2;
                if(ChessBoard[temxi][temyi].color!=0)
                    return false;
                return true;
            }
        }
        else
        {
            if(yi<=5)
                return false;
            if(!(abs(yi-select_yi)==2&&abs(xi-select_xi)==2))
                return false;
            int temxi=select_xi+(xi-select_xi)/2;
            int temyi=select_yi+(yi-select_yi)/2;
            if(ChessBoard[temxi][temyi].color!=0)
                return false;
            return true;
        }
    }

    if(check==shi)
    {
        if(!(abs(xi-select_xi)==1&&abs(yi-select_yi)==1))
            return false;
        if((mycolor==0)&&(current_color==1)||mycolor==1||mycolor==2)
        {
            if(!(xi>=3&&xi<=5&&yi<=9&&yi>=7))
                return false;
        }
        else
        {
            if(!(xi>=3&&xi<=5&&yi>=0&&yi<=2))
                return false;
        }
        return true;
    }
    if(check==jiang)
    {
        if(select_xi==xi&&ChessBoard[xi][yi].type==jiang)
        {
            if(select_yi-yi>3)
            {
                int count=0;
                for(int i=select_yi-1;i>=yi+1;i--)
                    if(ChessBoard[xi][i].color!=0)
                        return false;

                return true;
            }
            if(yi-select_yi>3)
            {
                int count=0;
                for(int i=yi-1;i>=select_yi+1;i--)
                    if(ChessBoard[xi][i].color!=0)
                        return false;

                return true;
            }

        }


        if(!((abs(xi-select_xi)==1&&abs(yi-select_yi)==0)||(abs(xi-select_xi)==0&&abs(yi-select_yi)==1)))
            return false;
        if(((mycolor==0)&&(current_color==1))||mycolor==1||mycolor==2)
        {
            if(!(xi>=3&&xi<=5&&yi<=9&&yi>=7))
                return false;
        }
        else
        {
            if(!(xi>=3&&xi<=5&&yi>=0&&yi<=2))
                return false;
        }
        return true;
    }


    /*保证网络对战的视角是不一样的*/
    return true;
}

void BattleArea::playaudio(int i)
{
    qDebug() << i;
    switch (i) {
    case 0:
        PLAY(SET_SOUND);
        musicplayer.play();

        break;
    case 1:
        QSound::play(":/audio/audio/capture.wav");

        break;
    case 2:
        QSound::play(":/audio/audio/checkmate.wav");
        break;
    case 3:
        QSound::play(":/audio/audio/win.wav");
        break;
    case 4:


        break;
    case 5:
        QSound::play(":/audio/audio/lose.mp3");
        break;
    case 6:
        QSound::play(":/audio/audio/win.wav");
        musicplayer.play();
        break;
    case 7:
        PLAY(MESSAGE_SOUND);
        musicplayer.play();
        break;
    case 8:
        QSound::play(":/audio/audio/illgeal.wav");
        musicplayer.play();
        break;
    case 9:
        PLAY(MESSAGE_SOUND);
        musicplayer.play();
        break;
    default:
        break;
    }
}

void BattleArea::setinfo(QString tem)
{
    ui->plainTextEdit->appendPlainText(tem);

}
//存档可以在本地玩
void BattleArea::savegame()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("存档"),
                               "：/",
                               tr("存档 (*.txt)"));

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
         {
          QMessageBox::critical(NULL,"提示","无法创建文件");
          return;

          }
    QTextStream out(&file);
    if(current_color==1)
        out << "red\n";
    {
        for(int i=3;i<=5;i++)
            for(int j=7;j<=9;j++)
                if(ChessBoard[i][j].type==jiang)
                    out << "1 "<< "<" << QString::number(i) <<","<< QString::number(9-j) << ">\n";
        vector<QPoint> tem;
        for(int i=3;i<=5;i++)
            for(int j=7;j<=9;j++)
                if(ChessBoard[i][j].type==shi)
                    tem.push_back(QPoint(i,9-j));

        out << QString::number(tem.size());

        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=5;j<=9;j++)
                if(ChessBoard[i][j].type==xiang&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ma&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ju&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==pao&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();

        for(int i=0;i<=8;i++)
            for(int j=0;j<=7;j++)
                if(ChessBoard[i][j].type==bing&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        out << "black\n";
        /*接下来存黑方*/
        for(int i=3;i<=5;i++)
            for(int j=0;j<=2;j++)
                if(ChessBoard[i][j].type==jiang&&ChessBoard[i][j].color==2)
                    out << "1 "<< "<" << QString::number(i) <<","<< QString::number(9-j) << ">\n";
        tem.clear();
        for(int i=3;i<=5;i++)
            for(int j=0;j<=2;j++)
                if(ChessBoard[i][j].type==shi&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));

        out << QString::number(tem.size());

        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=4;j++)
                if(ChessBoard[i][j].type==xiang&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ma&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ju&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==pao&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();

        for(int i=0;i<=8;i++)
            for(int j=0;j<=7;j++)
                if(ChessBoard[i][j].type==bing&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        out << "red\n";
    {
        for(int i=3;i<=5;i++)
            for(int j=7;j<=9;j++)
                if(ChessBoard[i][j].type==jiang)
                    out << "1 "<< "<" << QString::number(i) <<","<< QString::number(9-j) << ">\n";
        vector<QPoint> tem;
        for(int i=3;i<=5;i++)
            for(int j=7;j<=9;j++)
                if(ChessBoard[i][j].type==shi)
                    tem.push_back(QPoint(i,9-j));

        out << QString::number(tem.size());

        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=5;j<=9;j++)
                if(ChessBoard[i][j].type==xiang&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ma&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ju&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==pao&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();

        for(int i=0;i<=8;i++)
            for(int j=0;j<=7;j++)
                if(ChessBoard[i][j].type==bing&&ChessBoard[i][j].color==1)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";





    }
    if(current_color==2)
    {
        out << "black\n";
        /*接下来存黑方*/
        for(int i=3;i<=5;i++)
            for(int j=0;j<=2;j++)
                if(ChessBoard[i][j].type==jiang&&ChessBoard[i][j].color==2)
                    out << "1 "<< "<" << QString::number(i) <<","<< QString::number(9-j) << ">\n";
        tem.clear();
        for(int i=3;i<=5;i++)
            for(int j=0;j<=2;j++)
                if(ChessBoard[i][j].type==shi&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));

        out << QString::number(tem.size());

        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=4;j++)
                if(ChessBoard[i][j].type==xiang&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ma&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";

        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==ju&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();
        for(int i=0;i<=8;i++)
            for(int j=0;j<=9;j++)
                if(ChessBoard[i][j].type==pao&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
        tem.clear();

        for(int i=0;i<=8;i++)
            for(int j=0;j<=7;j++)
                if(ChessBoard[i][j].type==bing&&ChessBoard[i][j].color==2)
                    tem.push_back(QPoint(i,9-j));
        out << QString::number(tem.size());
        for(int i=0;i<tem.size();i++)
        {
            out << " <" << QString::number(tem[i].x()) << "," << QString::number(tem[i].y()) << ">";
        }
        out << "\n";
    }


    file.close();                    //只能以红色的格式存档


}

}
void BattleArea::loadgame(QString tem) //本机加载
{
    for(int i=0;i<=8;i++)
        for(int j=0;j<=9;j++)
        {
            ChessBoard[i][j].color=0;
            ChessBoard[i][j].type =none;
        }

    /*
    QString color_line=in.readLine();
    if(color_line==red)
        current_color==1;


    int color=current_color;  //存档时候用的

    QString line = in.readLine();
    line.remove('<');line.remove(',');line.remove('>');line.remove(' ');
    vector<QPoint> tem;
    tem.clear();
    int num=line[0].digitValue();
    if(num!=0)
    {
        int xi;
        int yi;
        for(int i=1;i<=num*2;i+=2)
        {
            xi=line[i].digitValue();
            yi=9-line[i+1].digitValue();
            tem.push_back(QPoint(xi,yi));
        }
        for(int i=0;i<tem.size();i++)
        {
            ChessBoard[xi][yi].type=jiang;
            ChessBoard[xi][yi].color=color;
        }
    }
    */

    tem.replace('\n',',');
    tem.remove('<');tem.remove(',');tem.remove('>');tem.remove(' ');
    int a=tem.indexOf('r');
    int b=tem.indexOf('b');
    if(a>b)
       tem.remove(0,b);
    else
       tem.remove(0,a);

    if(tem[0]=="r")
    {tem=tem.remove(0,3);current_color=1;}
    else
    {tem=tem.remove(0,5);current_color=2;}


    int color=current_color;
    int l1=tem[0].digitValue();

    QString ll1=tem.left(2*l1+1);
    tem.remove(0,2*l1+1);

    vector<QPoint> point;
    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }

        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=jiang;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }


    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=shi;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }


    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);


    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=xiang;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }

    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=ma;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }

    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

   point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=ju;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }

    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=pao;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }


    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=bing;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }


////////////////////////////////////////////////////////////
    if(color==1)
        tem.remove(0,5);
    else
        tem.remove(0,3);
    color=3-color;
    l1=tem[0].digitValue();
    ll1=tem.mid(0,2*l1+1);
            tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=jiang;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }


    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

   point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=shi;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }


    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=xiang;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }

    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=ma;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }

    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

   point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=ju;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }

    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);
    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=pao;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }


    l1=tem[0].digitValue();
    ll1=tem.left(2*l1+1);
        tem.remove(0,2*l1+1);

    point.clear();
    if(l1!=0)
    {
        int xi;int yi;
        for(int i=1;i<=l1*2;i+=2)
        {
            if(mycolor==1||mycolor==0)
            {
                xi=ll1[i].digitValue();
                yi=9-ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
            else
            {
                xi=8-ll1[i].digitValue();
                yi=ll1[i+1].digitValue();
                point.push_back(QPoint(xi,yi));
            }
        }
        for(int i=0;i<point.size();i++)
        {
            ChessBoard[point[i].x()][point[i].y()].type=bing;
            ChessBoard[point[i].x()][point[i].y()].color=color;

        }
    }



}






