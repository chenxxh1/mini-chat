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
    void sendToWidget(QByteArray byte);
public slots:
    void clientInfo();
private:
    QTcpSocket *socket;
signals:
};

#endif // MYTHREAD_H
