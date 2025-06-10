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
    //qDebug()<<byte;
    emit sendToWidget(byte,this);//将信息与socket发送到主线程,同时加上线程指针，用于传输
    //当前线程的一些信息,以便判断所属客户端
}
void Mythread::handleDisconnected(){
    QString ip =socket->peerAddress().toString();
    QString port =QString::number(socket->peerPort());
    QString message =QString("ip:%1,port:%2已断开服务器").arg(ip,port);
    emit clientDisconnected(message.toUtf8(),this);
}
QTcpSocket* Mythread::getSocket(){
    return this->socket;
}
void Mythread::setAccount(QString account){
    this->account=account;
}
QString Mythread::getAccount(){
    return this->account;
}
