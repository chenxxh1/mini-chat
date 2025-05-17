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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_aggreButton_clicked()
{
    if(ui->aggreButton->isChecked()){//判断同意隐私协议
        ui->LogButton->setStyleSheet("background-color: rgb(0, 170, 255);");//登入按钮变为蓝色
        ui->LogButton->setEnabled(1);//同意则登录按钮可以使用
    }else{
         ui->LogButton->setStyleSheet("background-color: rgb(170, 233, 255);");//登入按钮为灰蓝色
        ui->LogButton->setEnabled(0);//不同意则不可以使用
    }
}


void MainWindow::on_LogButton_clicked()
{
    qDebug()<<"登录";//调试输出窗口打印“登录”
    QWidget* secondwindow = new QWidget();
    secondwindow->show();
    this->close();
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

