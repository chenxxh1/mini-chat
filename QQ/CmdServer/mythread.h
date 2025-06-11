#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>
class Mythread : public QThread
{
    Q_OBJECT
public:
    explicit Mythread(QTcpSocket *s);
    void run();
    QTcpSocket* getSocket();
    void setAccount(QString account);
    QString getAccount();
signals:
    void sendToServer(QByteArray byte,Mythread *t);
    void clientDisconnected(QByteArray byte,Mythread *t);
public slots:
    void clientInfo();
    void handleDisconnected();
private:
    QTcpSocket *socket;
    QString account;
signals:
};

#endif // MYTHREAD_H
