#include "mythread.h"

Mythread::Mythread(QTcpSocket *s)
{
    socket =s;
}
void Mythread::run(){
    connect(socket,&QTcpSocket::readyRead,this,&Mythread::clientInfo);
    connect(socket,&QTcpSocket::disconnected,this,&Mythread::handleDisconnected);
}
void Mythread::clientInfo(){
    QByteArray byte =socket->readAll();
    qDebug()<<byte;
    emit sendToWidget(byte,socket);//将信息与socket发送到主线程
}
void Mythread::handleDisconnected(){
    QString ip =socket->peerAddress().toString();
    QString port =QString::number(socket->peerPort());
    QString message =QString("ip:%1,port:%2已断开服务器").arg(ip,port);
    emit clientDisconnected(message.toUtf8());
}
