#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QProgressBar>
#include <QTextStream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->show();
    this->ui->mainToolBar->hide();
    this->setWindowTitle("中国象棋 by Xianyu.G");
    initializeDialog=nullptr;
    Server_waitingDialog=nullptr;
    Server_connectingDialog=nullptr;
    Client_settingDialog=nullptr;
    edit_port=nullptr;
    client_ip=nullptr;
    client_port=nullptr;
    listenSocket=new QTcpServer;
    readWriteSocket=new QTcpSocket;
    battlearea=nullptr;
    if_select_netloading=false;
    checkbox=nullptr;


}

void MainWindow::init_local()
{
    initializeDialog->close();
    battlearea=new BattleArea(0,this);
    this->setCentralWidget(battlearea);
    battlearea->show();
    this->ui->mainToolBar->show();
    connect(battlearea,&BattleArea::move,battlearea,&BattleArea::playaudio);
    connect(battlearea,&BattleArea::gameover,this,&MainWindow::return_to_initialize);

}
void MainWindow::initialize()
{
    listenSocket->blockSignals(false);

    on_actionRefresh_triggered();
    if(!loadcontent.isEmpty())
        loadcontent.clear();
    initializeDialog=new QDialog(this);
    initializeDialog->setModal(true);
    initializeDialog->setAttribute(Qt::WA_DeleteOnClose);
    QLabel *label_0=new QLabel;
    QPushButton *button_1=new QPushButton;
    QPushButton *button_2=new QPushButton;
    QPushButton *button_3=new QPushButton;
    QVBoxLayout *layout=new QVBoxLayout;
    QHBoxLayout *layout_1= new QHBoxLayout;

    label_0->setText(tr("请选择开始方式"));
    button_1->setText(tr("创建房间"));
    button_2->setText(tr("进入房间"));
    button_3->setText(tr("本地游戏"));
    layout_1->addWidget(button_1);
    layout_1->addWidget(button_2);
    layout_1->addWidget(button_3);
    layout->addWidget(label_0);
    layout->addLayout(layout_1);
    initializeDialog->setLayout(layout);
    connect(button_1,SIGNAL(clicked()),this,SLOT(init_Server()));
    connect(button_2,SIGNAL(clicked()),this,SLOT(init_Client()));
    connect(button_3,SIGNAL(clicked()),this,SLOT(init_local()));
    checkbox=new QCheckBox;
    checkbox->setChecked(false);
    if_select_netloading=false;
    initializeDialog->show();
    initializeDialog->exec();


}
void MainWindow::init_Server()
{
    initializeDialog->close();
    Server_waitingDialog=new QDialog(this);
    Server_waitingDialog->setModal(1);

    checkbox->setText("是否加载存档");
    Server_waitingDialog->setAttribute(Qt::WA_DeleteOnClose);
    QLabel *label_1=new QLabel;
    QLabel *label_2=new QLabel;
    QPushButton *button =new QPushButton;
    edit_port =new QLineEdit;
    QVBoxLayout *layout =new QVBoxLayout;
    QHBoxLayout *layout_1=new QHBoxLayout;
    QHBoxLayout *layout_2=new QHBoxLayout;
    QComboBox* hostCombo =new QComboBox;
    hostCombo->setEditable(true);
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    edit_port->setValidator(new QIntValidator(10000, 65535, this));

    //address=ipAddr;
    //address="183.172.104.247";


    connect(hostCombo, &QComboBox::editTextChanged,
            [&](QString tem){
                address=tem;

            });


    label_2->setText(tr("设置端口号:"));
    button->setText(tr("确定"));
    layout_1->addWidget(label_2);
    layout_1->addWidget(edit_port);
    layout->addLayout(layout_1);
    layout->addWidget(hostCombo);
    layout_2->addWidget(button);
    layout_2->addWidget(checkbox);
    layout->addLayout(layout_2);
    Server_waitingDialog->setLayout(layout);
    int seed = QDateTime::currentDateTime().time().second();
        srand(seed);
    edit_port->setText(QString::number(qrand()%100+10000));
    connect(button,SIGNAL(clicked(bool)),this,SLOT(init_Server_2()));
    //this->listenSocket =new QTcpServer;
    Server_waitingDialog->show();
    Server_waitingDialog->exec();




}

void MainWindow:: init_Server_2()
{
    Server_waitingDialog->close();
    if(checkbox->isChecked())
         if_select_netloading=true;
    else
        if_select_netloading=false;

    if(if_select_netloading==true)
    {
        QString filename = QFileDialog::getOpenFileName(this, tr("读档"),
                                   "：/",
                                   tr("读档 (*.txt)"));
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
             {
              QMessageBox::critical(NULL,"提示","无法读取文件");
              return;

              }


         QTextStream in(&file);
         loadcontent=in.readAll();

    }
    else
    {loadcontent.clear();}



    Server_connectingDialog=new QDialog(this);
    Server_connectingDialog->setModal(1);
    Server_connectingDialog->setAttribute(Qt::WA_DeleteOnClose);
    QLabel *label_1=new QLabel;
    QLabel *label_2=new QLabel;
    QLabel *label_3=new QLabel;
    QString sentence_1;
    QString sentence_2;
    sentence_1.append("你的IP地址是:");
    sentence_1.append(address);
    label_2->setText(sentence_1);
    sentence_2.append("你的端口号是:");
    sentence_2.append(edit_port->text());
    label_3->setText(sentence_2);
    label_1->setText(tr("等待连接..."));
    QVBoxLayout *layout_1 = new QVBoxLayout;
    QHBoxLayout *layout_1_1=new QHBoxLayout;
    QPushButton *cancel=new QPushButton;
    cancel->setText("CANCEL");
    layout_1->addWidget(label_2);
    layout_1->addWidget(label_3);
    layout_1_1->addWidget(label_1);
    layout_1_1->addWidget(cancel);
    layout_1->addLayout(layout_1_1);
    Server_connectingDialog->setLayout(layout_1);
    QString port = edit_port->text();
    int portInt =port.toInt();

    this->listenSocket->listen(QHostAddress::Any,portInt);
    connect(listenSocket,&QTcpServer::newConnection,this,&MainWindow::Server_waitforidentification);


    connect(cancel,&QPushButton::clicked,[=]
                {
                    //delete label_1;delete label_2;delete label_3;delete layout_1;delete layout_1_1;delete edit_port;
                    listenSocket->blockSignals(true);
                    listenSocket->disconnect();

                    Server_connectingDialog->deleteLater();
                    Server_connectingDialog->close();
                    initialize();

                    return;

                });
     Server_connectingDialog->show();
     Server_connectingDialog->exec();







}


void MainWindow::init_Client()
{
    initializeDialog->close();
    Client_settingDialog =new QDialog(this);
    Client_settingDialog->setModal(1);
    Client_settingDialog->setAttribute(Qt::WA_DeleteOnClose);
    QLabel *label_1=new QLabel;
    QLabel *label_2=new QLabel;
    client_ip=new QLineEdit;
    client_port=new QLineEdit;
    QPushButton *button=new QPushButton;
    QPushButton *cancel=new QPushButton;
    QHBoxLayout *layout_1=new QHBoxLayout;
    QHBoxLayout *layout_2=new QHBoxLayout;
    QVBoxLayout *layout  =new QVBoxLayout;



    label_1->setText("对方的ip地址");
    label_2->setText("对方的端口号");
    button->setText("Confirm");
    cancel->setText("Cancel");
    layout_1->addWidget(label_1);
    layout_1->addWidget(client_ip);
    layout_2->addWidget(label_2);
    layout_2->addWidget(client_port);
    client_ip->setText("127.0.0.1");
    layout->addLayout(layout_1);
    layout->addLayout(layout_2);
    layout->addWidget(button);
    layout->addWidget(cancel);

    Client_settingDialog->setLayout(layout);
    //this->readWriteSocket=new QTcpSocket;
    connect(button,SIGNAL(clicked()),this,SLOT(init_Client_2()));
    connect(cancel,&QPushButton::clicked,[this]
    {
        Client_settingDialog->deleteLater();
        Client_settingDialog->close();
        initialize();
        return;
    });
    if_select_netloading=false;
    Client_settingDialog->show();
    Client_settingDialog->exec();



}
void MainWindow::recvMessage()
{
    QString info;
    info=readWriteSocket->readAll();
    if(info[0]=="C")
    {
        if(info=="CHINACHESSSUCCESS")
        {emit connecting_success();return;
        }

        QString first=info.left(11);
        info.remove(0,11);

        QString length=info.left(3);
            if(length=="000")
                if_select_netloading=false;
            else
            {
                info.remove(3);
                loadcontent=info;
                if_select_netloading=true;
            }
            if(first=="CHINACHESSr")
                emit connecting_success();
            return;
    }


    this->battlearea->getString(info);


}
void MainWindow::sendMessage(QString tem)
{
    QByteArray *array =new QByteArray;
    array->clear();
    array->append(tem);
    this->readWriteSocket->write(array->data());
    readWriteSocket->flush();


}

void MainWindow::init_Client_2()
{
    //Client_settingDialog->close();
    QString address = client_ip->text();
    int port=client_port->text().toInt();
    qDebug() << address << port;
    readWriteSocket->connectToHost(QHostAddress(address),port);


    connect(readWriteSocket,&QTcpSocket::readyRead,this,&MainWindow::recvMessage);
    connect(this,&MainWindow::connecting_success,this,&MainWindow::Client_connected);
    //connect(readWriteSocket,&QTcpSocket::connected,this,&MainWindow::Client_connected);
   // connect(readWriteSocket,&QTcpSocket::connected,Client_settingDialog,&QDialog::close);

}
void MainWindow::Client_connected()
{
    this->setWindowTitle("中国象棋_CLIENT");
    QByteArray *array =new QByteArray;
    array->clear();
    array->append("CHINACHESSSUCCESS");

    this->readWriteSocket->write(array->data());
    readWriteSocket->flush();

    battlearea=new BattleArea(2,this);    //客户端对应的是2，黑色
    this->setCentralWidget(battlearea);
    if(if_select_netloading==true)
        battlearea->loadgame(loadcontent);
    else
        {}

    if(if_select_netloading==false)
        battlearea->setinfo("模式：新建游戏");
    else
        battlearea->setinfo("模式：网络残局");
    connect(battlearea,&BattleArea::gameover,this,&MainWindow::return_to_initialize);
    connect(battlearea,&BattleArea::SendMessage,readWriteSocket,&QTcpSocket::flush);
    connect(readWriteSocket,&QTcpSocket::readyRead,this,&MainWindow::recvMessage);
    connect(battlearea,&BattleArea::SendMessage,this,&MainWindow::sendMessage);
    connect(readWriteSocket,&QTcpSocket::disconnected,[this]
                 {
                    QMessageBox msgBox;
                    msgBox.setText("Connection LOST!");
                    msgBox.exec();
                    battlearea->mycolor=0;
                    battlearea->setinfo("进入单机模式");
                  });
    this->ui->mainToolBar->show();
    Client_settingDialog->close();

}
void MainWindow::Server_waitforidentification()
{
    this->readWriteSocket=this->listenSocket->nextPendingConnection();
    QByteArray *array =new QByteArray;
    array->clear();
    if(if_select_netloading==true)
    {

        array->clear();
        array->append("CHINACHESSr");
        QString b=QString("%1").arg(loadcontent.size(), 3,10, QChar('0'));
        array->append(b);
        array->append(loadcontent);
        this->readWriteSocket->write(array->data());
        //readWriteSocket->flush();

    }
    else
    {   array->clear();
        array->append("CHINACHESSr000");
        this->readWriteSocket->write(array->data());
        readWriteSocket->flush();
    }

    connect(readWriteSocket,&QTcpSocket::readyRead,this,&MainWindow::recvMessage);
    connect(this,&MainWindow::connecting_success,this,&MainWindow::Server_acceptConnection);

}
void MainWindow::Server_acceptConnection()
{
     this->setWindowTitle("中国象棋_SERVER");
        battlearea=new BattleArea(1,this);  //服务器端设置为1，红色

     if(if_select_netloading)
        battlearea->loadgame(loadcontent);
    this->setCentralWidget(battlearea);
    Server_connectingDialog->close();
    battlearea->setinfo("Connected!");
    if(!if_select_netloading)
        battlearea->setinfo("模式：新建游戏");
    else
        battlearea->setinfo("模式：网络残局");
    connect(battlearea,&BattleArea::gameover,this,&MainWindow::return_to_initialize);
    connect(battlearea,&BattleArea::SendMessage,readWriteSocket,&QTcpSocket::flush);
    connect(battlearea,&BattleArea::SendMessage,this,&MainWindow::sendMessage);
    connect(readWriteSocket,&QTcpSocket::readyRead,this,&MainWindow::recvMessage);
    connect(readWriteSocket,&QTcpSocket::disconnected,[this]
                 {
                    QMessageBox msgBox;
                    msgBox.setText("Connection LOST!");
                    msgBox.exec();
                    battlearea->mycolor=0;
                    battlearea->setinfo("进入单机模式");
                    //ui->mainToolBar->hide();

                  });

    this->ui->mainToolBar->show();
    Server_connectingDialog->close();
}
void MainWindow::return_to_initialize()
{
    if(battlearea->mycolor!=0)
        disconnect();

}
void MainWindow::on_pushButton_clicked()
{
    initialize();
}

void MainWindow::on_actionN_triggered()
{

    if(battlearea!=nullptr)
    {
        disconnect();
        delete battlearea;battlearea=nullptr;ui->mainToolBar->hide();

    }




    initialize();
}

void MainWindow::on_ActionFail_triggered()
{
    battlearea->surrender();
}
void MainWindow::disconnect(int i)
{

   readWriteSocket->disconnectFromHost();
   listenSocket->disconnect();

   if_select_netloading=false;
   if(loadcontent.isEmpty())
    loadcontent.clear();
   battlearea->mycolor=0;


}

void MainWindow::on_ActionSave_triggered()
{
    if(battlearea->mycolor==1||battlearea->mycolor==0)
        battlearea->savegame();
    else
    {
        QMessageBox msgBox;
        msgBox.setText("客户端不能存档！！！");
        msgBox.exec();
    }

}

void MainWindow::on_ActionLoad_triggered()  //单机版
{
    if(battlearea->mycolor==0)
    {
        QString filename = QFileDialog::getOpenFileName(this, tr("读档"),
                                                        "：/",
                                                        tr("读档 (*.txt)"));

        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        {
            QMessageBox::critical(NULL,"提示","无法读取文件");
            return;

        }


        QTextStream in(&file);
        QString tem=in.readAll();
        //ui->plainTextEdit->appendPlainText(tem);

        battlearea->loadgame(tem);
    }
    else
    {
        QMessageBox::critical(NULL,"提示","游戏过程中无法读档");
        return;
    }

}


void MainWindow::on_actionRefresh_triggered()
{

    //delete listenSocket;
    //delete readWriteSocket;
    QTcpServer* tem1=listenSocket;
    QTcpSocket* tem2=readWriteSocket;
    tem2->flush();


    if(!loadcontent.isEmpty())
        loadcontent.clear();
    listenSocket=new QTcpServer;
    readWriteSocket=new QTcpSocket;
    listenSocket->close();
    //tem1->deleteLater();
   // tem2->deleteLater();
    if_select_netloading=false;



}

void MainWindow::on_pushButton_2_clicked()
{
    QWidget* info=new QWidget;
    info->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout* layout=new QVBoxLayout;
    QLabel* label=new QLabel;
    label->setAlignment(Qt::AlignHCenter);
    label->setText("本程序创建房间的时候可以选择加载残局！！！\n客户端在网络连接完成的时候收到残局信息！！！\n房主可以保存残局！！！\n自动进入本地模式！！！\n没有悔棋！！！\n感谢tty同学的协议！！！可以跨平台对战！！！\n享受游戏吧！！\n\nXianyu.G");
    label->setStyleSheet("QLabel{color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(0, 0, 0, 255), stop:0.05 rgba(14, 8, 73, 255), stop:0.36 rgba(28, 17, 145, 255), stop:0.6 rgba(126, 14, 81, 255), stop:0.75 rgba(234, 11, 11, 255), stop:0.79 rgba(244, 70, 5, 255), stop:0.86 rgba(255, 136, 0, 255), stop:0.935 rgba(239, 236, 55, 255));font: 22pt 'MicroSoft Yahei';}");
    layout->addWidget(label);
    info->setLayout(layout);
    info->show();







}
