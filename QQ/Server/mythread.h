#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
class Mythread : public QThread
{
    Q_OBJECT
public:
    explicit Mythread(QTcpSocket *s);
    void run();
    QTcpSocket* getSocket();
    void setJO(QJsonObject jo);
    QJsonObject getJO();
signals:
    void sendToWidget(QByteArray byte,Mythread *t);
    void clientDisconnected(QByteArray byte,Mythread *t);
public slots:
    void clientInfo();
    void handleDisconnected();
private:
    QTcpSocket *socket;
    QJsonObject jsonOb;
signals:
};

#endif // MYTHREAD_H
