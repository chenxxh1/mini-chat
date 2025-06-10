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
    bool isAccountExists(QString account);
    void findallfriend(const QString &account,QJsonObject &response);
    QString crypassword(QString password);
    bool insertFrinend(QString ,QString,int);
    ~Server();
    void setPrintInterval(int seconds); // 添加设置打印间隔的方法
public slots:
    void newClient();
    void newMessageReciver(QByteArray byte,Mythread *currentThread);
    void disClient(QByteArray byte,Mythread *t);
    void printOnlineAccounts();

private:
    QTcpServer *server;
    QSqlDatabase db;
    QMap<QString,Mythread*>threadInfo;
    QMap<QString, QTcpSocket*> userSocketMap;
    QTimer *timer; // 添加定时器
};

#endif // SERVER_H
