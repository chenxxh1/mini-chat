#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlError>
#include "mythread.h"
#include <QMap>
#include <QCryptographicHash>
#include <QTimer>
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    bool isAccountExists(const QString &account);
    QJsonArray findallfriend(const QString &account);
    QJsonArray findallgroup(const QString &account);
    void find(const QString &account,QJsonObject& response);
    void sendToClient(QTcpSocket* socket,const QJsonObject& response);
    QString crypassword(const QString &password);
    bool insertFriend(const QString& ,const QString&,const int&);
    ~Server();
    void setPrintInterval(const int& seconds); // 添加设置打印间隔的方法
    void initDatabaseTables();
    void checkTableExists(const QString &tableName);
public slots:
    void newClient();
    void newMessageReciver(QByteArray byte,Mythread *currentThread);
    void disClient(QByteArray byte,Mythread *t);
    void printOnlineAccounts();

private:
    QTcpServer *server;
    QSqlDatabase db;
    QMap<QString,Mythread*>threadInfo;//存储所有在线的账号对应的线程
    QMap<QString, QTcpSocket*> userSocketMap;
    QTimer *timer; // 添加定时器
};

#endif // SERVER_H
