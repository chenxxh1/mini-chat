#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    server = new QTcpServer;
    server->listen(QHostAddress::AnyIPv4,8000);
    db =QSqlDatabase ::addDatabase("QMYSQL");
    db.setDatabaseName("mydb");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("root");
    if(db.open()){
        QMessageBox::information(this,"提示","数据库连接成功");
    }else{
        QMessageBox::information(this,"提示","数据库连接失败");
    }

    connect(server,&QTcpServer::newConnection,this,&Widget::newClient);
}

Widget::~Widget()
{
    delete ui;
}
void Widget::newClient(){
    QTcpSocket *newsocket =server->nextPendingConnection();
    QString ip =newsocket->peerAddress().toString();
    QString port =QString::number(newsocket->peerPort());
    QString message =QString("ip:%1,port:%2").arg(ip,port);
    ui->messageList->addItem(message);
    Mythread *t =new Mythread(newsocket);
    t->start();
    connect(t,&Mythread::sendToWidget,this,&Widget::newMessageReciver);
}
void Widget::newMessageReciver(QByteArray byte){
    QString message =QString(byte);
    ui->messageList->addItem(message);
}
