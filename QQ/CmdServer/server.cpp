#include "server.h"
#include <QRandomGenerator>
Server::Server (QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer;
    server->listen(QHostAddress::AnyIPv4,8000);
    qDebug() << "Server started on port 8000";


    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydb.sqlite");
    if (db.open()) {
        qDebug() << "Database connected successfully";
    } else {
        qDebug() << "Database connection failed:" << db.lastError().text();
    }
    connect(server,&QTcpServer::newConnection,this,&Server::newClient);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Server::printOnlineAccounts);
    timer->start(30000);
    setPrintInterval(30);
    initDatabaseTables();

    QSqlQuery query;
    query.prepare("select * from users;");
    if(query.exec()){
        qDebug()<<"all users:";
        while(query.next()){
            qDebug()<<query.value(0)<<query.value(1)<<query.value(2);
        }
    }
}
void Server::initDatabaseTables() {
    QSqlQuery query;

    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "account TEXT NOT NULL UNIQUE, "
               "password TEXT NOT NULL, "
               "nickname TEXT NOT NULL)");

    query.exec("CREATE TABLE IF NOT EXISTS friendships ("
               "user1_account TEXT, "
               "user2_account TEXT, "
               "status INTEGER, "
               "PRIMARY KEY (user1_account, user2_account))");

    query.exec("CREATE TABLE IF NOT EXISTS messages ("
               "sender TEXT, "
               "receiver TEXT, "
               "content TEXT, "
               "time DATETIME, "
               "message_id VARCHAR(36) UNIQUE)");

    checkTableExists("users");
    checkTableExists("friendships");
    checkTableExists("messages");
}

void Server::checkTableExists(const QString &tableName) {
    QSqlQuery checkQuery;
    QString checkQueryStr = QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'")
                                .arg(tableName);//sqlite_master 是 SQLite 数据库中一个内置的表，用于存储数据库的元数据，比如表名、索引名等。

    checkQuery.exec(checkQueryStr);

    if (checkQuery.next()) {
        qDebug() << "Table '" << tableName << "' exists.";
    } else {
        qDebug() << "Table '" << tableName << "' does not exist.";
    }
}

void Server::setPrintInterval(const int &seconds)
{
    if (seconds > 0) {
        timer->setInterval(seconds * 1000);
        timer->start();
        qDebug() << "Online accounts will be printed every" << seconds << "seconds";
    } else {
        qWarning() << "Invalid interval value. Printing disabled";
        timer->stop();
    }
}
Server ::~Server ()
{
    delete server;
    delete timer;
    for (auto i=threadInfo.begin();i!=threadInfo.end();++i){
        delete i.value();
    }
}

bool Server ::isAccountExists(const QString& account){
    QSqlQuery ifaccountexits;
    ifaccountexits.prepare("SELECT 1 FROM users WHERE account = :account");
    ifaccountexits.bindValue(":account", account);
    if(ifaccountexits.exec()){
        return ifaccountexits.next();
    } else {
        qDebug() << "Query failed:" << ifaccountexits.lastError().text();
        return false;
    }
}

QJsonArray Server::findallfriend(const QString &account)
{
    qDebug()<<"账号："<<account;
    qDebug()<<"好友列表:";
    QSqlQuery query;
    QJsonArray allfriend;
    query.prepare("SELECT user2_account, status FROM friendships WHERE user1_account = :account AND status = 1 "
                  "UNION SELECT user1_account, status FROM friendships WHERE user2_account = :account;");
    query.bindValue(":account", account);

    if (query.exec()) {
        while (query.next()) {
            QString friend_account = query.value(0).toString();
            QSqlQuery qu;
            QString friend_nickname;

            qu.prepare("SELECT nickname FROM users WHERE account = :account");
            qu.bindValue(":account", friend_account);

            if (qu.exec()) {
                if (qu.next()) {
                    friend_nickname = qu.value(0).toString();
                }
            }

            int status = query.value(1).toInt();
            QJsonObject one_friend;
            one_friend["friend_account"] = friend_account;
            one_friend["friend_nickname"] = friend_nickname;
            one_friend["status"] = status;

            allfriend.append(one_friend);
        }
        qDebug() << allfriend;
    }

    return allfriend;
}

QJsonArray Server::findallgroup(const QString &account){
    qDebug()<<"账号："<<account;
    qDebug()<<"群列表:";
    QJsonArray allgroup;
    return allgroup;
}

void Server::find(const QString &account,QJsonObject &response)
{
    response["type"] = "update_resopnse";
    response["allfriend"] = findallfriend(account);
    response["allgroup"]=findallgroup(account);

}

void Server::sendToClient(QTcpSocket *socket, const QJsonObject &response)
{
    QByteArray byte =QJsonDocument(response).toJson();
    socket->write(byte);
}

QString Server::crypassword(const QString &password)
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


    QString shortenedResult = QString(result.toHex()).left(19);

    return shortenedResult;
}

bool Server::insertFriend(const QString &v_account, const QString &account, const int &status)
{
    QSqlQuery query;
    query.prepare("INSERT INTO friendships(user1_account, user2_account, status) VALUES (:v_account, :account, :status);");
    query.bindValue(":v_account", v_account);
    query.bindValue(":account", account);
    query.bindValue(":status", status);

    if (query.exec()) {
        qDebug() << "插入成功";
        return true;
    } else {
        qDebug() << "插入失败：" << query.lastError().text();
    }
    return false;
}

void Server ::newClient(){
    QTcpSocket *newsocket = server->nextPendingConnection();
    QString ip = newsocket->peerAddress().toString();
    QString port = QString::number(newsocket->peerPort());
    QString message = QString("ip:%1, port:%2 connected").arg(ip, port);
    qDebug() << message;

    Mythread *t = new Mythread(newsocket);
    t->start();
    connect(t, &Mythread::sendToServer, this, &Server::newMessageReciver);
    connect(t, &Mythread::clientDisconnected, this, &Server::disClient);
}
void Server ::newMessageReciver(QByteArray byte,Mythread *currentThread){
    QTcpSocket *socket =currentThread->getSocket();

    QJsonObject response;

    QJsonDocument receiverdocument = QJsonDocument::fromJson(byte);

    qDebug()<<"接受的信息:"<<receiverdocument;
    if(!receiverdocument.isNull()&&receiverdocument.isObject()){
        QJsonObject receiverjsonObject =receiverdocument.object();
        QString type =receiverjsonObject.value("type").toString();
        qDebug()<<receiverjsonObject;
        if(type=="login"){
            QString account = receiverjsonObject.value("account").toString();
            QString password = receiverjsonObject.value("password").toString();
            password=crypassword(password);
            QSqlQuery query;
            query.prepare("SELECT * FROM users WHERE account = :account");
            query.bindValue(":account", account);
            if(query.exec()){
                response["type"] = "login_response";
                if(query.next()){
                    int storeId = query.value(0).toInt();
                    QString storeAccount = query.value(1).toString();
                    QString storePassword = query.value(2).toString();
                    QString snickName = query.value(3).toString();

                    QJsonObject jsonObject;
                    jsonObject["id"] = storeId;
                    jsonObject["account"] = storeAccount;
                    jsonObject["nickname"] = snickName;
                    if(storePassword==password){

                        response["status"] = "success";
                        response["message"] = "Login successful";

                        response["account"]=storeAccount;
                        response["nickname"]=snickName;
                        response["id"]=storeId;
                        currentThread->setAccount(storeAccount);
                        threadInfo[storeAccount]=currentThread;
                        userSocketMap[storeAccount] = socket;
                    }else{

                        response["status"] = "failure";
                        response["message"] = "Incorrect password";
                    }
                }else{

                    response["status"] = "failure";
                    response["message"] = "Account not found";
                }
            }

        }else if(type=="register"){

            QString account = QString::number(QRandomGenerator::global()->bounded(static_cast<double>(1000000000LL)) + 10000000, 'f', 0);
            while(isAccountExists(account)){
                QString account = QString::number(QRandomGenerator::global()->bounded(static_cast<double>(1000000000LL)) + 10000000, 'f', 0);
            }
            QString nickname =receiverjsonObject.value("nickname").toString();
            QString password = receiverjsonObject.value("password").toString();
            password=crypassword(password);

            QSqlQuery query;
            query.prepare("INSERT INTO users (account, password, nickname) VALUES (?, ?, ?)");
            query.addBindValue(account);
            query.addBindValue(password);
            query.addBindValue(nickname);

            qDebug() << "SQL Query:" << query.lastQuery();
            qDebug() << "Bound Parameters: account=" << account << ", password=" << password << ", nickname=" << nickname;

            response["type"] = "register_response";
            if (query.exec()) {

                QString successMessage = QString("Registration successful: Account: %1, Nickname: %2").arg(account, nickname);
                qDebug()<<successMessage;


                response["status"] = "success";
                response["message"] = "Registration successful";
                response["account"] = account; // 返回生成的账号


                insertFriend(account,account,1);
            } else {
                QString errorMessage = QString("Registration failed: Error: %1")
                .arg(query.lastError().text());
                qDebug()<<errorMessage;
                response["type"] = "register_response";
                response["status"] = "failure";
                response["message"] = query.lastError().text();
            }

        }else if(type=="addFriend_search"){
            QString message=receiverjsonObject.value("message").toString();

            QSqlQuery query_n;
            query_n.prepare("SELECT account,nickname FROM users WHERE nickname = :message");
            query_n.bindValue(":message", message);
            QSqlQuery query_a;
            query_a.prepare("SELECT account,nickname FROM users WHERE account = :message");
            query_a.bindValue(":message", message);
            QJsonArray usersArray;
            if(query_n.exec()){
                while(query_n.next()){

                    QString account = query_n.value("account").toString();
                    QString nickname = query_n.value("nickname").toString();


                    QJsonObject userObject;
                    userObject["account"] = account;
                    userObject["nickname"] = nickname;


                    usersArray.append(userObject);
                }
            }
            if (query_a.exec()) {
                while (query_a.next()) {

                    QString account = query_a.value("account").toString();
                    QString nickname = query_a.value("nickname").toString();

                    QJsonObject userObject;
                    userObject["account"] = account;
                    userObject["nickname"] = nickname;


                    usersArray.append(userObject);
                }
            }
            response["type"]="addFriend_searcher_reponse";
            response["users"]=usersArray;
        }else if (type == "checkFriend") {
            qDebug() << "checkFriend";
            response["type"] = "checkFriend_response";

            QString v_account = receiverjsonObject["v_account"].toString();
            QString account = receiverjsonObject["account"].toString();

            QSqlQuery query;
            query.prepare("SELECT COUNT(*) AS is_friend FROM friendships WHERE "
                          "(user1_account = :v_account AND user2_account = :account AND status = 1) OR "
                          "(user2_account = :v_account AND user1_account = :account AND status = 1);");
            query.bindValue(":v_account", v_account);
            query.bindValue(":account", account);

            if (query.exec()) {
                if (query.next()) {
                    int is_friend = query.value(0).toInt();
                    if (is_friend > 0) {
                        response["result"] = "is friend";
                    } else {
                        response["result"] = "is not friend";
                    }
                }
            } else {
                qDebug() << "Query failed:" << query.lastError().text();
            }
        }else if(type=="friend_request"){
            response["type"]="friend_request_response";
            QString v_account=receiverjsonObject["v_account"].toString();
            QString account=receiverjsonObject["account"].toString();
            if(insertFriend(v_account,account,0)){
                response["result"]="insert_successed";
            }else{
                response["result"]="insert_not_successed";
            }

            auto it = threadInfo.find(account);
            if (it != threadInfo.end()) {
                qDebug() << account<<"在线";
                Mythread* f_thread = it.value();
                QTcpSocket* f_socket=f_thread->getSocket();
                QJsonObject f_response;
                find(account,f_response);
                sendToClient(f_socket,f_response);
            } else {
                qDebug() << account<<"未在线";
            }
        }else if(type=="update"){
            QString account =receiverjsonObject["account"].toString();
            find(account,response);
        }else if(type=="Agree_the_friend"){
            QString account = receiverjsonObject["account"].toString();
            QString friend_account = receiverjsonObject["friend_account"].toString();
            qDebug() << "account:" << account << "friend_account:" << friend_account;
            QSqlQuery query;
            query.prepare("UPDATE friendships SET status = 1 WHERE user1_account = ? AND user2_account = ?;");
            query.addBindValue(friend_account);
            query.addBindValue(account);
            if (query.exec()) {
                qDebug() << "Agree_the_friend" << query.lastQuery();
                find(account,response);

                auto it = threadInfo.find(friend_account);
                if (it != threadInfo.end()) {
                    qDebug() << friend_account<<"在线";
                    Mythread* f_thread = it.value();
                    QTcpSocket* f_socket=f_thread->getSocket();
                    QJsonObject f_response;
                    find(friend_account,f_response);
                    sendToClient(f_socket,f_response);
                } else {
                    qDebug() << friend_account<<"未在线";
                }
            } else {
                qDebug() << "Update failed:" << query.lastError().text();
            }
            qDebug() << "Agree_the_friend" << query.lastQuery();
        }else if (type=="Refuse_the_friend"||type=="Delete_the_friend"){
            QString account = receiverjsonObject["account"].toString();
            QString friend_account = receiverjsonObject["friend_account"].toString();
            qDebug() << "account:" << account << "friend_account:" << friend_account;

            QSqlQuery query;
            query.prepare("DELETE FROM friendships WHERE user1_account = :friend_account AND user2_account = :account;");
            query.bindValue(":account", account);
            query.bindValue(":friend_account", friend_account);

            if (query.exec()) {
                qDebug() << "Delete successful";
                find(account,response);

                auto it = threadInfo.find(friend_account);
                if (it != threadInfo.end()) {
                    qDebug() << friend_account<<"在线";
                    Mythread* f_thread = it.value();
                    QTcpSocket* f_socket=f_thread->getSocket();
                    QJsonObject f_response;
                    find(friend_account,f_response);
                    sendToClient(f_socket,f_response);
                } else {
                    qDebug() << friend_account<<"未在线";
                }
            } else {
                qDebug() << "Delete failed:" << query.lastError().text();
            }
        }
        else if(type == "chat_message"){
            QString from = receiverjsonObject["from"].toString();
            QString to = receiverjsonObject["to"].toString();
            qDebug() << "to_user:::" << from << to;
            QString content = receiverjsonObject["content"].toString();
            QString time = receiverjsonObject["time"].toString();

            QJsonObject chatData;
            chatData["type"] = "chat_message";
            chatData["from"] = from;
            chatData["to"] = to;
            chatData["content"] = content;
            chatData["time"] = time;

            QJsonDocument doc(chatData);
            QByteArray data = doc.toJson();


            QSqlQuery saveChat;
            saveChat.prepare("INSERT INTO messages (sender, receiver, content, time) VALUES (:from, :to, :content, :time)");
            saveChat.bindValue(":from", from);
            saveChat.bindValue(":to", to);
            saveChat.bindValue(":content", content);
            saveChat.bindValue(":time", time);

            if (!saveChat.exec()) {
                qDebug() << "Failed to save message:" << saveChat.lastError().text();
            } else {
                qDebug() << "Message saved successfully";
            }


            if (userSocketMap.contains(to)) {
                userSocketMap[to]->write(data);
                response["status"] = "sent";
            } else {
                response["status"] = "offline";
            }
        }
        else if(type == "get_history") {
            QString a1 = receiverjsonObject["from"].toString();
            QString a2 = receiverjsonObject["to"].toString();

            QSqlQuery query;
            query.prepare("SELECT sender, receiver, content, time FROM messages WHERE "
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
                    msg["name"] = nicknameMap.value(sender, sender);//发送者的名称
                    msg["from"] = sender;
                    msg["to"] = query.value("receiver").toString();
                    msg["content"] = query.value("content").toString();
                    msg["time"] = query.value("time").toString();
                    msgArray.append(msg);
                }
            }

            response["type"] = "get_history_response";
            response["messages"] = msgArray;


        } else if(type == "create_group") {
            QString groupName = receiverjsonObject["group_name"].toString();
            QString ownerAccount = receiverjsonObject["owner_account"].toString();


            QSqlQuery query;
            query.prepare("INSERT INTO `groups` (group_name, owner_account) VALUES (:group_name, :owner_account)");
            query.bindValue(":group_name", groupName);
            query.bindValue(":owner_account", ownerAccount);

            response["type"] = "create_group_response";

            if (query.exec()) {
                qint64 groupId = query.lastInsertId().toLongLong();


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
    sendToClient(socket,response);
}
void Server ::disClient(QByteArray byte,Mythread *t){
    QString message =QString(byte);
    qDebug()<<message;
    QString account =t->getAccount();
    threadInfo.remove(account);
    userSocketMap.remove(account);

}

void Server::printOnlineAccounts()
{
    if(threadInfo.isEmpty()) {
        qDebug() << "[Online Accounts] No clients connected";
        return;
    }

    qDebug().noquote() << "\n===== Online Accounts =====";
    qDebug().noquote() << "Total:" << threadInfo.size() << "accounts";

    for (auto i = threadInfo.begin(); i != threadInfo.end(); ++i) {
        QString account = i.key();
        Mythread* thread = i.value();
        QTcpSocket* socket = thread->getSocket();

        QString status = "Active";
        if(socket->state() != QTcpSocket::ConnectedState) {
            status = "Disconnected";
        }

        QString ip = socket->peerAddress().toString();
        QString port = QString::number(socket->peerPort());

        qDebug().noquote() << QString("Account: %1 | IP: %2:%3 | Status: %4")
                                  .arg(account, ip, port, status);
    }
    qDebug().noquote() << "===========================\n";
}

