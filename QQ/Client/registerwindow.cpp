#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "dragevent.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
Register::Register(QTcpSocket *s,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    socket=s;
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->Close,&QToolButton::clicked,this,&Register::on_Close_triggered);
}

Register::~Register()
{
    delete ui;
}
void Register::on_Back_clicked()
{
    emit R_close();
}


void Register::on_Close_triggered()
{
    emit R_close();
}


void Register::on_RegisterIn_clicked()
{
    QString name =ui->Name->text();
    QString firstP =ui->PassWord->text();
    QString secondP =ui->ConfirmWord->text();
    if(firstP!=secondP){
        QMessageBox::warning(this,"警告","两次输入不同");
    }else{
        QJsonObject jsonObject;
        jsonObject["nickname"]=name;
        jsonObject["password"]=firstP;
        jsonObject["type"]="register";//数据类型
        QJsonDocument document(jsonObject);
        QByteArray byte =document.toJson();
        socket->write(byte);
    }
}

