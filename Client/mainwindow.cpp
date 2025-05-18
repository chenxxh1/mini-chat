#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->CloseButton,&QToolButton::clicked,this,&MainWindow::on_CloseButton_triggered);
    socket =new QTcpSocket;
    socket->connectToHost("127.0.0.1",8000);
    connect(socket,&QTcpSocket::connected,this,[this](){
        QMessageBox::information(this,"提示","连接服务器成功");
    });
    connect(socket,&QTcpSocket::disconnected,this,[this](){
        QMessageBox::warning(this,"警告","连接服务器失败");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_aggreButton_clicked()
{
    //登录按钮的变色逻辑
    if(ui->aggreButton->isChecked()){
        ui->LogButton->setStyleSheet("background-color: rgb(0, 170, 255);");
        ui->LogButton->setEnabled(1);
    }else{
         ui->LogButton->setStyleSheet("background-color: rgb(170, 233, 255);");
        ui->LogButton->setEnabled(0);
    }
}


void MainWindow::on_LogButton_clicked()//登录
{
    QString account =ui->accountEdit->text();
    QString password =ui->passwordEdit->text();
    QString accInfo =QString("account:%1 password:%2").arg(account,password);
    qDebug()<<accInfo;
    QByteArray data=accInfo.toUtf8();
    socket->write(data);
}


void MainWindow::on_CloseButton_triggered()
{
    //qDebug()<<"close";
    this->close();
}

