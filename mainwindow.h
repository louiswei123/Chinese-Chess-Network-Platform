#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QWidget>
#include <QtNetwork>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <vector>
#include <algorithm>
#include <QStringList>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <stdlib.h>
#include <QPalette>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <battlearea.h>
#include <QComboBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initialize();                        //选择进入服务器还是进入客户端
;
   // ~MainWindow();
public slots:
    void init_local();
    void init_Server();
    void init_Server_2();
    void Server_acceptConnection();
    void Server_waitforidentification();
    void init_Client();
    void init_Client_2();
    void Client_connected();
    void recvMessage();
    void sendMessage(QString tem);

    void return_to_initialize();
    void disconnect(int i=0);   //分为客户端和服务器端
     void on_actionRefresh_triggered();
signals:
    void connecting_success();


private slots:
    void on_pushButton_clicked();

    void on_actionN_triggered();

    void on_ActionFail_triggered();

    void on_ActionSave_triggered();

    void on_ActionLoad_triggered();

   // void on_actionInfo_triggered();

   // void on_actionLoad_triggered();



    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QDialog *initializeDialog;
    QDialog *validating_dialog;
    QDialog *Server_waitingDialog;
    QDialog *Server_connectingDialog;
    QDialog *Client_settingDialog;
    QString address;
    QLineEdit* edit_port;
    QLineEdit* client_ip;
    QLineEdit* client_port;
    QTcpServer *listenSocket;
    QTcpSocket *readWriteSocket;
    BattleArea *battlearea;
    bool volatile if_select_netloading;

    QString loadcontent;
    QCheckBox* checkbox;

};

#endif // MAINWINDOW_H
