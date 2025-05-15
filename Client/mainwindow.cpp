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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_aggreButton_clicked()
{
    if(ui->aggreButton->isChecked()){
        ui->LogButton->setStyleSheet("background-color: rgb(0, 170, 255);");
        ui->LogButton->setEnabled(1);
    }else{
         ui->LogButton->setStyleSheet("background-color: rgb(170, 233, 255);");
        ui->LogButton->setEnabled(0);
    }
}


void MainWindow::on_LogButton_clicked()
{
    qDebug()<<"登录";
}


void MainWindow::on_CloseButton_triggered()
{
    //qDebug()<<"close";
    this->close();
}

