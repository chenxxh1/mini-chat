#include "widget.h"
#include "ui_widget.h"
#include <QRandomGenerator>
QString generateMessageId();
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer;
    server->listen(QHostAddress::AnyIPv4,8000);
    db =QSqlDatabase ::addDatabase("QMYSQL");
    db.setDatabaseName("mychat");
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
        return query.next(); // 如果找到了匹配的记录，返回 1，否则返回 空 即为假
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
    //所以第一个搜索中加上status=1,进行限制,以此得到好友申请和好友关系
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
    qDebug() << __func__ << response;
}
void Widget::findallgroup(const QString &account,QJsonObject &response)
{
QSqlQuery query;
    qDebug()<<account;

    query.prepare(R"(
    SELECT g.group_id, g.group_name, g.owner_account
    FROM `groups` g
    JOIN group_members gm ON g.group_id = gm.group_id
    WHERE gm.account = :account
)");



query.bindValue(":account", account);

QJsonArray groupArray;
if (query.exec()) {
    while (query.next()) {
        QJsonObject group;
        group["group_id"] = query.value("group_id").toString();
        group["group_name"] = query.value("group_name").toString();
        group["owner_account"] = query.value("owner_account").toString();
        groupArray.append(group);
    }
} else {
    qDebug() << "Failed to fetch groups:" << query.lastError().text();
}

response["type"] = "find_all_groups_response";
response["groups"] = groupArray;
qDebug() << __func__ << response;
}
QString Widget::crypassword(QString password)
{
    QCryptographicHash Md5(QCryptographicHash::Md5);
    QCryptographicHash Sha256(QCryptographicHash::Sha256);
    QCryptographicHash RealSha3_384(QCryptographicHash::RealSha3_384);
    Md5.addData(password.toUtf8());
    QByteArray result = Md5.result();
    Sha256.addData(result);
    result = Sha256.result();
    RealSha3_384.addData(result);
    result = RealSha3_384.result();

    // 截取前19个字符
    QString shortenedResult = QString(result.toHex()).left(19);

    return shortenedResult;
}

bool Widget::insertFrinend(QString v_account, QString account, int status)
{
    QSqlQuery query;
    query.prepare("insert into friendships(user1_account,user2_account,status) values(:v_account,:account,:status);");
    query.bindValue(":v_account",v_account);
    query.bindValue(":account",account);
    query.bindValue(":status",status);
    if(query.exec()){
        qDebug()<<"插入成功";
        return true;
    }else{
        qDebug()<<query.lastError();
    }
    return false;
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
    qDebug() << byte;
    QJsonDocument document = QJsonDocument::fromJson(byte);
    qDebug() << document;
    if(!document.isNull()&&document.isObject()){
        QJsonObject jsonObject =document.object();
        QString type =jsonObject.value("type").toString();
        qDebug()<<jsonObject;
        if(type=="login"){
            QString account = jsonObject.value("account").toString();
            QString password = jsonObject.value("password").toString();
            password=crypassword(password);
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
                        currentThread->setAccount(storeAccount);
                        threadInfo[storeAccount]=currentThread;//将登录成功的信息加入线程信息
                        userSocketMap[storeAccount] = socket;
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

        }else if(type=="register"){

            QString account = QString::number(QRandomGenerator::global()->bounded(1000000000000ll)+10000000);
            while(isAccountExists(account)){
                account = QString::number(QRandomGenerator::global()->bounded(1000000000000ll)+10000000);
            }
            QString nickname =jsonObject.value("nickname").toString();
            QString password = jsonObject.value("password").toString();
            password=crypassword(password);

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

                //将自己与自己建立好友关系
                insertFrinend(account,account,1);

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
            if(insertFrinend(v_account,account,0)){
                response["result"]="insert_successed";
            }else{
                response["result"]="insert_not_successed";
            }
        }else if(type=="View_friend_relationships"){
            QString account =jsonObject["account"].toString();
            findallfriend(account,response);

        }else if(type=="View_Group_relationships"){
            QString account =jsonObject["account"].toString();
            findallgroup(account,response);

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
        }else if (type=="Refuse_the_friend"||type=="Delete_the_friend"){
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
        else if (type == "chat_message") {
            QString from = jsonObject["from"].toString();
            QString to = jsonObject["to"].toString();
            qDebug() << "to_user:::" << from << to;
            QString content = jsonObject["content"].toString();
            QString time = jsonObject["time"].toString(); // 可以加时间字段

            // 生成唯一消息ID
            QString messageId = QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);

            QJsonObject chatData;
            chatData["type"] = "chat_message";
            chatData["id"] = messageId;   // 添加唯一 ID
            chatData["from"] = from;
            chatData["to"] = to;
            chatData["content"] = content;
            chatData["time"] = time;

            QJsonDocument doc(chatData);
            QByteArray data = doc.toJson();

            QSqlQuery saveChat;
            saveChat.prepare("INSERT INTO messages (sender, receiver, content, time, message_id) "
                             "VALUES (:from, :to, :content, :time, :messageId)");
            saveChat.bindValue(":from", from);
            saveChat.bindValue(":to", to);
            saveChat.bindValue(":content", content);
            saveChat.bindValue(":time", time);
            saveChat.bindValue(":messageId", messageId);  // 插入唯一ID

            if (!saveChat.exec()) {
                qDebug() << "Failed to save message:" << saveChat.lastError().text();
            }

            if(userSocketMap.contains(to)) {
                userSocketMap[to]->write(data); // 转发给对方
                response["status"] = "sent";
            } else {
                response["status"] = "offline"; // 对方不在线
            }
        }
        else if (type == "get_history") {
            QString a1 = jsonObject["from"].toString();
            QString a2 = jsonObject["to"].toString();

            QSqlQuery query;
            query.prepare("SELECT sender, receiver, content, time, message_id FROM messages WHERE "
                          "(sender = :a1 AND receiver = :a2) OR (sender = :a2 AND receiver = :a1) "
                          "ORDER BY time ASC");
            query.bindValue(":a1", a1);
            query.bindValue(":a2", a2);

            QMap<QString, QString> nicknameMap;
            QSqlQuery nicknameQuery;
            nicknameQuery.prepare("SELECT account, nickname FROM users WHERE account = :a1 OR account = :a2");
            nicknameQuery.bindValue(":a1", a1);
            nicknameQuery.bindValue(":a2", a2);
            if (nicknameQuery.exec()) {
                while (nicknameQuery.next()) {
                    QString account = nicknameQuery.value("account").toString();
                    QString nickname = nicknameQuery.value("nickname").toString();
                    nicknameMap[account] = nickname;
                }
            }

            QJsonArray msgArray;
            if (query.exec()) {
                while (query.next()) {
                    QString sender = query.value("sender").toString();
                    QJsonObject msg;
                    msg["id"] = query.value("message_id").toString();      // 添加唯一 ID
                    msg["name"] = nicknameMap.value(sender, sender);       // 发送者的名称
                    msg["from"] = sender;
                    msg["to"] = query.value("receiver").toString();
                    msg["content"] = query.value("content").toString();
                    msg["time"] = query.value("time").toString();
                    msgArray.append(msg);
                }
            }

            response["type"] = "get_history_response";
            response["messages"] = msgArray;
        }
        else if(type == "create_group") {
            QString groupName = jsonObject["group_name"].toString();
            QString ownerAccount = jsonObject["owner_account"].toString();

            // 创建群组
            QSqlQuery query;
            query.prepare("INSERT INTO `groups` (group_name, owner_account) VALUES (:group_name, :owner_account)");
            query.bindValue(":group_name", groupName);
            query.bindValue(":owner_account", ownerAccount);

            response["type"] = "create_group_response";

            if (query.exec()) {
                qint64 groupId = query.lastInsertId().toLongLong();

                // 插入群主到群成员表，role设为'owner'
                QSqlQuery addMember;
                addMember.prepare("INSERT INTO group_members (group_id, account, role) VALUES (:group_id, :account, 'owner')");
                addMember.bindValue(":group_id", groupId);
                addMember.bindValue(":account", ownerAccount);
                addMember.exec();

                response["status"] = "success";
                response["group_id"] = QString::number(groupId);
                response["message"] = "Group created successfully";
                if (!addMember.exec()) {
                    qDebug() << "Failed to add group member:" << addMember.lastError().text();
                } else {
                    qDebug() << "Added group member successfully.";
                }
            } else {
                response["status"] = "failure";
                response["message"] = query.lastError().text();
            }

        }


    }
    responseData = QJsonDocument(response).toJson();
    socket->write(responseData);
}
void Widget::disClient(QByteArray byte,Mythread *t){
    QString message =QString(byte);
    ui->messageList->addItem(message);
    QString account =t->getAccount();
    threadInfo.remove(account);
    userSocketMap.remove(account);
    //qDebug()<<account;
}

void Widget::on_accountpushButton_clicked()
{
    ui->accountList->clear();
    for (auto i = threadInfo.begin(); i != threadInfo.end(); ++i) {
        // 获取账户名和线程指针
        QString account =i.key();
        //Mythread* currentThread = i.value();
        // 创建一个列表项
        QListWidgetItem *item = new QListWidgetItem();
        // 设置列表项的文本
        QString text = account;
        item->setText(text);

        // 将列表项添加到 QListWidget
        ui->accountList->addItem(item);

        // 将列表项添加到 QListWidget
        ui->accountList->addItem(item);
    }
}
#include <QUuid>
#include <QJsonObject>

QString generateMessageId() {
    return QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces);
}
