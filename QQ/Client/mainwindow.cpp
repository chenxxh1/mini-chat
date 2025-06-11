#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dragevent.h"
#include "registerwindow.h"
const QString publicIp = "47.111.134.228"; // 替换为你的公网 IP
const QString localIp = "127.0.0.1";   // 本地 IP
const int port = 8000;                 // 端口号
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
    connectToServer();
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::serverSocket);
    connect(ui->RegisterButton,&QPushButton::clicked,this,&MainWindow::RegisterButton_clicked);

}
void MainWindow::connectToServer() {
    // 尝试连接公网 IP
    socket->connectToHost("127.0.0.1", port);

    // 连接成功时的处理
    connect(socket, &QTcpSocket::connected, this, [this](){
        QMessageBox::information(this, "提示", "连接服务器成功");
    });

    //接失败时的处理
    connect(socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError socketError){
        if (socketError == QAbstractSocket::ConnectionRefusedError) {
            // 如果连接公网 IP 失败，尝试连接本地 IP
            socket->connectToHost(localIp, port);
            QMessageBox::information(this, "提示", "连接本地服务器成功");
        } else {
            // 其他错误，提示用户
            QMessageBox::warning(this, "警告", "连接服务器失败");
        }
    });


    // 服务器断开连接时的处理
    connect(socket, &QTcpSocket::disconnected, this, [this](){
        QMessageBox::warning(this, "警告", "服务器断开");
    });
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
    // 将 JSON 对象转换为 QByteArray
    QJsonDocument document(jsonObject);
    QByteArray data = document.toJson();
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
        qDebug()<<"接受的信息："<<jsonObject;
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
                   ||type=="View_friend_relationships_response"
                   ||type == "get_history_response"
                   ||type=="chat_message"
                   ||type=="create_group_response"
                   ||type=="group_chat_message"
                   ||type=="update_resopnse"){
            emit sendToIN(jsonObject);
        }
    }
}

