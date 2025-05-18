#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerwindow.h"

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
<<<<<<< HEAD
    //登录按钮的变色逻辑
    if(ui->aggreButton->isChecked()){
        ui->LogButton->setStyleSheet("background-color: rgb(0, 170, 255);");
        ui->LogButton->setEnabled(1);
=======
    if(ui->aggreButton->isChecked()){//判断同意隐私协议
        ui->LogButton->setStyleSheet("background-color: rgb(0, 170, 255);");//登入按钮变为蓝色
        ui->LogButton->setEnabled(1);//同意则登录按钮可以使用
>>>>>>> e2c865ef229b5ce2ac50f1cc09534df2cb18a934
    }else{
         ui->LogButton->setStyleSheet("background-color: rgb(170, 233, 255);");//登入按钮为灰蓝色
        ui->LogButton->setEnabled(0);//不同意则不可以使用
    }
}


void MainWindow::on_LogButton_clicked()//登录
{
<<<<<<< HEAD
    QString account =ui->accountEdit->text();
    QString password =ui->passwordEdit->text();
    QString accInfo =QString("account:%1 password:%2").arg(account,password);
    qDebug()<<accInfo;
    QByteArray data=accInfo.toUtf8();
    socket->write(data);
=======
    qDebug()<<"登录";//调试输出窗口打印“登录”
    QWidget* secondwindow = new QWidget();
    secondwindow->show();
    this->close();
>>>>>>> e2c865ef229b5ce2ac50f1cc09534df2cb18a934
}


void MainWindow::on_CloseButton_triggered()
{
    //qDebug()<<"close";
    this->close();
}


void MainWindow::on_RejisterButton_clicked()
{
    Register* regis = new Register;
    this->hide();
    regis->show();
}

