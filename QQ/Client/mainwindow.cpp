#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dragevent.h"
#include "registerwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    r=nullptr;
    index=nullptr;
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->CloseButton,&QToolButton::clicked,this,&MainWindow::on_CloseButton_triggered);
    socket =new QTcpSocket;
    socket->connectToHost("127.0.0.1",8000);
    connect(socket,&QTcpSocket::connected,this,[this](){
        QMessageBox::information(this,"提示","连接服务器成功");
    });
    connect(socket,&QTcpSocket::disconnected,this,[this](){
        QMessageBox::warning(this,"警告","服务器断开");
    });

    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::serverSocket);
    connect(ui->RegisterButton,&QPushButton::clicked,this,&MainWindow::RegisterButton_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete r;
    //delete socket;
    delete index;
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

void MainWindow::on_LogButton_clicked() // 登录
{
    QString account = ui->accountEdit->text();
    QString password = ui->passwordEdit->text();

    // 创建一个 JSON 对象来存储账号和密码
    QJsonObject jsonObject;
    jsonObject["account"] = account;
    jsonObject["password"] = password;
    jsonObject["type"]="login";//数据类型
    jsonObject["senderIp"]=socket->localAddress().toString();
    jsonObject["senderPort"]=QString::number(socket->localPort());
    jsonObject["receiverIp"]=socket->peerAddress().toString();
    jsonObject["receiverPort"]=QString::number(socket->peerPort());
    // 将 JSON 对象转换为 QByteArray
    QJsonDocument document(jsonObject);
    QByteArray data = document.toJson();

    // 打印调试信息
    //qDebug() << "Sending data:" << data;

    // 发送数据
    socket->write(data);
}


void MainWindow::on_CloseButton_triggered()
{
    qDebug()<<"close";
    this->close();
}


void MainWindow::RegisterButton_clicked()
{
    //QMessageBox::information(this,"提示","调用");
    this->hide();
    r =new Register(socket,this);
    r->show();
    connect(r,&Register::R_close,this,&MainWindow::againShow);
}
void MainWindow::againShow(){
    this->show();
    r->close();
}

void MainWindow::serverSocket(){
    QByteArray byte =socket->readAll();
    QJsonDocument document =QJsonDocument::fromJson(byte);
    if(!document.isNull()&&document.isObject()){
        QJsonObject jsonObject =document.object();
        qDebug()<<__func__<<jsonObject;
        QString type = jsonObject.value("type").toString();
        if(type=="register_response"){
            QString status =jsonObject.value("status").toString();
            if(status=="success"){
                QString account =jsonObject.value("account").toString();
                QString inf =QString("注册成功,账号:%1").arg(account);
                QMessageBox::
                    information(this,"提示",inf);
                //返回登录界面
                r->close();
                ui->accountEdit->setText(account);
                this->show();
            }
            else
                QMessageBox::warning(this,"提示","注册失败");
        }else if(type=="login_response"){
            QString status =jsonObject.value("status").toString();
            if(status=="success"){
                jsonOb["account"]=jsonObject.value("account").toString();
                jsonOb["nickname"]=jsonObject.value("nickname").toString();
                jsonOb["id"]=jsonObject.value("id").toString();
                this->hide();
                index =new Index(socket,jsonOb,this);//主
                connect(index,&Index::I_close,this,[this](){
                    this->show();
                });
                connect(this,&MainWindow::sendToIN,index,&Index::frommain);
                index->show();

            }else{
                QString message =jsonObject.value("message").toString();
                QMessageBox::information(this,"提示",message);
            }
        }else if(type=="addFriend_searcher_reponse"
                   ||type=="checkFriend_response"
                   ||type=="friend_request_response"
                   ||type=="View_friend_relationships_response"||type == "get_history_response"){
            emit sendToIN(jsonObject);
        }
    }
}

