#include "widget.h"
#include "ui_widget.h"
#include "mythread.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer;
    server->listen(QHostAddress::AnyIPv4,8000);
    db =QSqlDatabase ::addDatabase("QMYSQL");
    db.setDatabaseName("mydb");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("root");
    if(db.open()){
        QMessageBox::information(this,"提示","数据库连接成功");
    }else{
        QMessageBox::information(this,"提示","数据库连接失败");
    }

    connect(server,&QTcpServer::newConnection,this,&Widget::newClient);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::newClient(){
    QTcpSocket *newsocket =server->nextPendingConnection();
    QString ip =newsocket->peerAddress().toString();
    QString port =QString::number(newsocket->peerPort());
    QString message =QString("ip:%1,port:%2已连接服务器").arg(ip,port);
    ui->messageList->addItem(message);
    Mythread *t =new Mythread(newsocket);
    t->start();
    connect(t,&Mythread::sendToWidget,this,&Widget::newMessageReciver);
    connect(t,&Mythread::clientDisconnected,this,&Widget::disClient);
}
void Widget::newMessageReciver(QByteArray byte,QTcpSocket *socket){
    //回显数据
    QJsonObject response;
    QByteArray responseData;
    //接受到的数据
    QJsonDocument document = QJsonDocument::fromJson(byte);
    if(!document.isNull()&&document.isObject()){
        QJsonObject jsonObject =document.object();
        QString type =jsonObject.value("type").toString();
        if(type=="login"){
            QString account = jsonObject.value("account").toString();
            QString password = jsonObject.value("password").toString();
            QString Seip =jsonObject.value("senderIp").toString();
            QString Seport=jsonObject.value("senderPort").toString();
            QString message = QString("Login request: Account: %1, Password: %2 "
                                      "IP: %2, PORT: %4").arg(account, password,Seip,Seport);

            ui->messageList->addItem(message);
            QSqlQuery query;
            query.prepare("SELECT password FROM users WHERE account = :account");
            query.bindValue(":account", account);

            if(query.exec()){
                response["type"] = "login_response";//消息类型
                if(query.next()){
                    QString storePassword =query.value(0).toString();
                    if(storePassword==password){
                        //密码正确
                        response["status"] = "success";
                        response["message"] = "Login successful";
                    }else{
                        //密码错误
                        response["status"] = "failure";
                        response["message"] = "Incorrect password";
                    }
                }else{
                    //无账号
                    response["status"] = "failure";
                    response["message"] = "Account not found";
                }
            }
            QString Logmessage=QString("%1, result:%2").arg(message,response.value("message").toString());
            ui->messageList->addItem(Logmessage);
            //返回登录信息
            responseData = QJsonDocument(response).toJson();
            socket->write(responseData);
        }else if(type=="register"){
            QString account = jsonObject.value("account").toString();
            QString password = jsonObject.value("password").toString();
            QString Seip =jsonObject.value("senderIp").toString();
            QString Seport=jsonObject.value("senderPort").toString();
            QString message = QString("Login request: Account: %1, Password: %2 "
                                      "IP: %2, PORT: %4").arg(account, password,Seip,Seport);
            ui->messageList->addItem(message);

            QSqlQuery query;
            query.prepare("INSERT INTO users (account, password) VALUES (:account, :password)");

            // 绑定参数
            query.bindValue(":account", account);
            query.bindValue(":password", password);
            QJsonObject response;
            response["type"] = "register_response";
            if (query.exec()) {
                //注册成功
                QString successMessage = QString("Registration successful: Account: %1").arg(account);
                ui->messageList->addItem(successMessage);
                response["status"] = "success";
                response["message"] = "Registration successful";
            } else {
                QString errorMessage = QString("Registration failed: Account: %1, Error: %2")
                .arg(account, query.lastError().text());
                ui->messageList->addItem(errorMessage);
                response["status"] = "failure";
                response["message"] = query.lastError().text();
            }
            //返回注册信息
            QByteArray responseData = QJsonDocument(response).toJson();
            socket->write(responseData);
        }
    }
}
void Widget::disClient(QByteArray byte){
    QString message =QString(byte);
    ui->messageList->addItem(message);
}
