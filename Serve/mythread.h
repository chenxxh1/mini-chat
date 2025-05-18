#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QDebug>
class Mythread : public QThread
{
    Q_OBJECT
public:
    explicit Mythread(QTcpSocket *s);
    void run();
signals:
    void sendToWidget(QByteArray byte,QTcpSocket *s);
    void clientDisconnected(QByteArray byte);
public slots:
    void clientInfo();
    void handleDisconnected();
private:
    QTcpSocket *socket;
signals:
};

#endif // MYTHREAD_H
