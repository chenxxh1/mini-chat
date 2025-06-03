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
    query.prepare("SELECT 1 FROM users WHERE account = :account"); // 查询常量值1
    query.bindValue(":account", account);
    if(query.exec()){
        return query.next(); // 如果找到了匹配的记录，返回 true，否则返回 false
    } else {
        qDebug() << "Query failed:" << query.lastError().text(); // 记录错误信息
        return false; // 查询失败，返回 false
    }
}

void Widget::findallfriend(const QString &account,QJsonObject &response)
{
    QSqlQuery query;
    QJsonArray allfriend;
    //user1_account为发送好友申请的账号
    //所以第一个搜索中加上status=1,进行限制
    query.prepare("select user2_account,status from friendships where user1_account=:account and status =1 "
                  "union select user1_account,status from friendships where user2_account=:account;");
    query.bindValue(":account",account);
    if(query.exec()){
        while(query.next()){
            QString friend_account=query.value(0).toString();
            QSqlQuery qu;
            QString friend_nickname;
            qu.prepare("select nickname from users where account = :account");
            qu.bindValue(":account",friend_account);
            if(qu.exec()){
                if(qu.next()){
                    friend_nickname=qu.value(0).toString();
                }
            }
            int status=query.value(1).toInt();
            QJsonObject one_friend;
            one_friend["friend_account"]=friend_account;
            one_friend["friend_nickname"]=friend_nickname;
            one_friend["status"]=status;
            allfriend.append(one_friend);
        }
        qDebug()<<allfriend;
    }
    response["type"]="View_friend_relationships_response";
    response["allfriend"]=allfriend;
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
        qDebug()<<type;
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

                    QJsonObject jsonObject;//账号信息
                    jsonObject["id"] = storeId;
                    jsonObject["account"] = storeAccount;
                    jsonObject["nickname"] = snickName;
                    if(storePassword==password){
                        //密码正确
                        response["status"] = "success";
                        response["message"] = "Login successful";
                        //将账号信息发送到客户端
                        response["account"]=storeAccount;
                        response["nickname"]=snickName;
                        response["id"]=storeId;
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
            //qDebug()<<"发送信息";

        }else if(type=="register"){

            QString account = QString::number(QRandomGenerator::global()->bounded(1000000000000ll)+10000000);
            while(isAccountExists(account)){
                //qDebug()<<account;
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
        }else if(type=="addFriend_search"){
            //信息
            // QString sendaccount=jsonObject.value("sendaccount").toString();
            // QString sendnickname=jsonObject.value("sendnickname").toString();
            QString message=jsonObject.value("message").toString();
            //在数据库中搜索
            //提供两种搜索方式，nickname，account
            QSqlQuery query_n;
            query_n.prepare("SELECT account,nickname FROM users WHERE nickname = :message");
            query_n.bindValue(":message", message);
            QSqlQuery query_a;
            query_a.prepare("SELECT account,nickname FROM users WHERE account = :message");
            query_a.bindValue(":message", message);
            QJsonArray usersArray;
            if(query_n.exec()){//通过nickanme搜索
                while(query_n.next()){
                    // 获取每一行的数据
                    QString account = query_n.value("account").toString();
                    QString nickname = query_n.value("nickname").toString();

                    // 创建一个 JSON 对象表示一个用户
                    QJsonObject userObject;
                    userObject["account"] = account;
                    userObject["nickname"] = nickname;

                    // 将用户对象添加到数组中
                    usersArray.append(userObject);
                }
            }
            if (query_a.exec()) {
                while (query_a.next()) {
                    // 获取每一行的数据
                    QString account = query_a.value("account").toString();
                    QString nickname = query_a.value("nickname").toString();
                    // 创建一个 JSON 对象表示一个用户
                    QJsonObject userObject;
                    userObject["account"] = account;
                    userObject["nickname"] = nickname;

                    // 将用户对象添加到数组中
                    usersArray.append(userObject);
                }
            }
            response["type"]="addFriend_searcher_reponse";
            response["users"]=usersArray;
        }else if(type=="checkFriend"){
            qDebug()<<"checkFriend";
            response["type"]="checkFriend_response";
            QString v_account=jsonObject["v_account"].toString();
            QString account=jsonObject["account"].toString();
            QSqlQuery query;
            query.prepare("select count(*) as is_friend from friendships where "
                          "(user1_account= :v_account and user2_account= :account and status=1) or "
                          "(user2_account= :v_account and user1_account= :account and status=1);");
            query.bindValue(":v_account",v_account);
            query.bindValue(":account",account);
            if(query.exec()){
                if(query.next()){
                    bool is_friend=query.value(0).toBool();
                    if(is_friend){
                        response["result"]="is friend";
                    }else{
                        response["result"]="is not friend";
                    }
                }
            }else{
                qDebug()<<query.lastQuery();
            }
        }else if(type=="friend_request"){
            response["type"]="friend_request_response";
            QString v_account=jsonObject["v_account"].toString();
            QString account=jsonObject["account"].toString();
            qDebug()<<v_account<<"  "<<account;
            QSqlQuery query;
            query.prepare("insert into friendships(user1_account,user2_account,status) values(:v_account,:account,0);");
            query.bindValue(":v_account",v_account);
            query.bindValue(":account",account);
            if(query.exec()){
                response["result"]="insert_successed";
                qDebug()<<"插入成功";
            }else{
                qDebug()<<query.lastError();
                response["result"]="insert_not_successed";
            }
        }else if(type=="View_friend_relationships"){
            QString account =jsonObject["account"].toString();
            findallfriend(account,response);
        }else if(type=="Agree_the_friend"){
            QString account=jsonObject["account"].toString();
            QString friend_account=jsonObject["friend_account"].toString();
            qDebug()<<"account:"<<account<<"friend_account:"<<friend_account;
            QSqlQuery query;
            query.prepare("update friendships set status=1 where user1_account=:friend_account and user2_account=:account;;");
            query.bindValue(":account",account);
            query.bindValue(":friend_account",friend_account);
            if(query.exec()){
                qDebug()<<"Agree_the_friend"<<query.lastQuery();
                findallfriend(account,response);
            }else{
                qDebug()<<query.lastError();
            }
            qDebug()<<"Agree_the_friend"<<query.lastQuery();
        }else if (type=="Refuse_the_friend"){
            QString account=jsonObject["account"].toString();
            QString friend_account=jsonObject["friend_account"].toString();
            QSqlQuery query;
            query.prepare("delete from friendships where user1_account=:friend_account and user2_account=:account;");
            query.bindValue(":account",account);
            query.bindValue(":friend_account",friend_account);
            if(query.exec()){
                findallfriend(account,response);
            }else{
                qDebug()<<query.lastError();
            }
        }
    }
    responseData = QJsonDocument(response).toJson();
    socket->write(responseData);
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

