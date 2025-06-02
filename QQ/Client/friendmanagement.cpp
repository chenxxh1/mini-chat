#include "friendmanagement.h"
#include "ui_friendmanagement.h"
#include "dragevent.h"
FriendManagement::FriendManagement(QTcpSocket *s,QJsonObject js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FriendManagement)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    socket=s;
    model=new QStandardItemModel(this);
    account =js["account"].toString();
    QJsonObject jsonobect;
    jsonobect["type"]="View_friend_relationships";
    jsonobect["account"]=account;
    QByteArray byte=QJsonDocument(jsonobect).toJson();
    socket->write(byte);

    connect(ui->closetoolButton,&QToolButton::clicked,this,[this](){
        this->close();
        emit FM_close();
    });
}
FriendManagement::~FriendManagement()
{
    delete ui;
}

void FriendManagement::fromIN(QJsonObject jsonobject){
    QString type=jsonobject["type"].toString();
    if (type=="View_friend_relationships_response"){
        if(jsonobject.contains("allfriend")&&jsonobject["allfriend"].isArray()){
            QJsonArray allfriend=jsonobject["allfriend"].toArray();
            for(QJsonArray::const_iterator it=allfriend.constBegin();it!=allfriend.constEnd();++it){
                const auto &item =*it;
                if (item.isObject()) {
                    QJsonObject cu_friend = item.toObject();
                    qDebug()<<cu_friend;
                    QString account = cu_friend["friend_account"].toString();
                    QString nickname = cu_friend["friend_nickname"].toString();
                    int status=cu_friend["status"].toInt();
                    //qDebug() << "User:" << account << nickname;
                    QString message=QString("账号：%1，昵称：%2,状态: %3").arg(account,nickname).arg(status);
                    //qDebug()<<message;
                    QStandardItem *item =new QStandardItem(message);
                    model->appendRow(item);
                }
            }
        }
        ui->messagelistView->setModel(model);
    }

}
