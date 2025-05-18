#include "mythread.h"

Mythread::Mythread(QTcpSocket *s)
{
    socket =s;
}
void Mythread::run(){
    connect(socket,&QTcpSocket::readyRead,this,&Mythread::clientInfo);
}
void Mythread::clientInfo(){
    QByteArray byte =socket->readAll();
    qDebug()<<byte;
    emit sendToWidget(byte);
}
