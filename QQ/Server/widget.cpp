#include "widget.h"
#include "ui_widget.h"
#include <QRandomGenerator>
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
    delete server;
}
bool Widget::isAccountExists(QString account){
    QSqlQuery query;
    query.prepare("SELECT 1 FROM users WHERE account = :account"); // 查询常量值1，而不是密码
    query.bindValue(":account", account);
    if(query.exec()){
        return query.next(); // 如果找到了匹配的记录，返回 true，否则返回 false
    } else {
        qDebug() << "Query failed:" << query.lastError().text(); // 记录错误信息
        return false; // 查询失败，返回 false
    }
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
void Widget::newMessageReciver(QByteArray byte,Mythread *currentThread){
    QTcpSocket *socket =currentThread->getSocket();
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
                                      "IP: %3, PORT: %4").arg(account, password,Seip,Seport);

            ui->messageList->addItem(message);
            QSqlQuery query;
            query.prepare("SELECT * FROM users WHERE account = :account");
            query.bindValue(":account", account);

            if(query.exec()){
                response["type"] = "login_response";//消息类型
                if(query.next()){
                    int storeId = query.value(0).toInt();
                    QString storeAccount = query.value(1).toString();
                    QString storePassword = query.value(2).toString();
                    QString snickName = query.value(3).toString();

                    QJsonObject jsonObject;
                    jsonObject["id"] = storeId;
                    jsonObject["account"] = storeAccount;
                    jsonObject["password"] = storePassword;
                    jsonObject["nickname"] = snickName;
                    if(storePassword==password){
                        //密码正确
                        response["status"] = "success";
                        response["message"] = "Login successful";

                        QJsonDocument jd(jsonObject);
                        currentThread->setJO(jsonObject);
                        threadInfo[jd.toJson(QJsonDocument::Compact)]=currentThread;//将登录成功的信息加入线程信息
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

            QString account = QString::number(QRandomGenerator::global()->bounded(1000000000000ll)+10000000);
            while(isAccountExists(account)){
                qDebug()<<account;
                account = QString::number(QRandomGenerator::global()->bounded(1000000000000ll)+10000000);
            }
            QString nickname =jsonObject.value("nickname").toString();
            QString password = jsonObject.value("password").toString();
            QString Seip =jsonObject.value("senderIp").toString();
            QString Seport=jsonObject.value("senderPort").toString();
            QString message = QString("Register request: Account: %1, Password: %2 "
                                      "IP: %3, PORT: %4").arg(account, password,Seip,Seport);
            ui->messageList->addItem(message);

            QSqlQuery query;
            query.prepare("INSERT INTO users (account, password, nickname) VALUES (:account, :password, :nickname)");
            query.bindValue(":account", account);
            query.bindValue(":password", password);
            query.bindValue(":nickname", nickname);
            QJsonObject response;
            response["type"] = "register_response";
            if (query.exec()) {
                //注册成功
                QString successMessage = QString("Registration successful: Account: %1, Nickname: %2").arg(account, nickname);
                ui->messageList->addItem(successMessage);

                // 构造成功响应消息
                response["status"] = "success";
                response["message"] = "Registration successful";
                response["account"] = account; // 返回生成的账号
            } else {
                QString errorMessage = QString("Registration failed: Error: %1")
                .arg(query.lastError().text());
                ui->messageList->addItem(errorMessage);
                response["type"] = "register_response";
                response["status"] = "failure";
                response["message"] = query.lastError().text();
            }
            //返回注册信息
            QByteArray responseData = QJsonDocument(response).toJson();
            socket->write(responseData);
        }
    }
}
void Widget::disClient(QByteArray byte,Mythread *t){
    QString message =QString(byte);
    ui->messageList->addItem(message);
    QJsonObject jsonObject =t->getJO();
    QJsonDocument jsonDocument(jsonObject);
    threadInfo.remove(jsonDocument.toJson(QJsonDocument::Compact));
    //qDebug()<<account;
}

void Widget::on_accountpushButton_clicked()
{
    qDebug()<<__func__;
    ui->accountList->clear();
    for (auto i = threadInfo.begin(); i != threadInfo.end(); ++i) {
        // 获取账户名和线程指针
        QJsonObject json =QJsonDocument::fromJson(i.key().toUtf8()).object();
        //Mythread* currentThread = i.value();
        // 创建一个列表项
        QListWidgetItem *item = new QListWidgetItem();
        int ID =json["id"].toInt();
        QString Account =json["account"].toString();
        QString Nickname =json["nickname"].toString();
        // 设置列表项的文本
        QString text = QString("ID: %1, Account: %2, Nickname: %3").arg(ID).arg(Account,Nickname);
        item->setText(text);

        // 将列表项添加到 QListWidget
        ui->accountList->addItem(item);

        // 将列表项添加到 QListWidget
        ui->accountList->addItem(item);
    }
}

