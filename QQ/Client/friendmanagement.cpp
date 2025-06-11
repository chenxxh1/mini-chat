#include "friendmanagement.h"
#include "ui_friendmanagement.h"
#include "dragevent.h"
#include "frienditemwidget.h"
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
    nickname=js["nickname"].toString();
    //update();
    connect(ui->closetoolButton,&QToolButton::clicked,this,[this](){
        this->close();
        emit FM_close();
    });
    connect(this,&FriendManagement::createItem,this,&FriendManagement::addFriendItem);
    ui->messagelistView->setModel(model);
    // 设置 QListView 的大小调整策略
    ui->messagelistView->setResizeMode(QListView::Adjust);
    ui->messagelistView->setUniformItemSizes(true);
}
void FriendManagement::update(){
    QJsonObject jsonobect;
    jsonobect["type"]="View_friend_relationships";
    jsonobect["account"]=account;
    QByteArray byte=QJsonDocument(jsonobect).toJson();
    socket->write(byte);
}
void FriendManagement::onAgreeButtonClicked(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    QJsonObject json=js;
    json["type"]="Agree_the_friend";
    json["account"]=account;
    QByteArray byte =QJsonDocument(json).toJson();
    socket->write(byte);
}

void FriendManagement::onRefuseButtonClicked(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    QJsonObject json=js;
    json["type"]="Refuse_the_friend";
    json["account"]=account;
    QByteArray byte =QJsonDocument(json).toJson();
    socket->write(byte);
}

void FriendManagement::onSendMessageButtonClicked(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    QString friendAccount = js["friend_account"].toString();
    QString friendName = js["friend_nickname"].toString();
    if(account==friendAccount){
        friendName="self";
    }
    ChatWindow *chat = new ChatWindow(socket, account, friendAccount,friendName, nullptr);
    connect(this,&FriendManagement::sendToCHAT,chat,&ChatWindow::onReadyRead);
    chat->show();
}

void FriendManagement::onDeleteButtonClicked(const QJsonObject &js){
    qDebug()<<__func__<<js;
    QJsonObject json=js;
    json["type"]="Delete_the_friend";
    json["account"]=account;
    QByteArray byte =QJsonDocument(json).toJson();
    socket->write(byte);
}
FriendManagement::~FriendManagement()
{
    delete ui;
}

void FriendManagement::addFriendItem(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    // 创建自定义小部件
    QJsonObject newjs=js;
    newjs["account"]=account;
    newjs["type"]="FRIENDMANAGEMENT";//用于判断是否显示出删除好友按钮
    FriendItemWidget* widget = new FriendItemWidget(newjs,this);
    widget->show();
    // 连接按钮点击信号
    connect(widget, &FriendItemWidget::deleteButtonClicked,this,&FriendManagement::onDeleteButtonClicked);
    connect(widget, &FriendItemWidget::agreeButtonClicked, this, &FriendManagement::onAgreeButtonClicked);
    connect(widget, &FriendItemWidget::refuseButtonClicked, this, &FriendManagement::onRefuseButtonClicked);
    connect(widget, &FriendItemWidget::sendMessageButtonClicked, this, &FriendManagement::onSendMessageButtonClicked);


    // 创建一个不可见的项，用于占据空间
    QStandardItem* item = new QStandardItem();
    item->setEditable(false);
    QSize sizeHint = item->sizeHint();
    sizeHint.setHeight(40); // 设置高度为40
    item->setSizeHint(sizeHint);
    // 将小部件添加到视图中
    int row = model->rowCount();
    qDebug()<<"row:"<<row;
    model->insertRow(row, item);
    qDebug()<<model->index(row,0);
    ui->messagelistView->setIndexWidget(model->index(row, 0), widget);
}

void FriendManagement::fromIN(QJsonObject jsonobject){
    qDebug()<<__func__<<jsonobject;
    QString type=jsonobject["type"].toString();
    if (type=="View_friend_relationships_response"){
        model->clear();
        if(jsonobject.contains("allfriend")&&jsonobject["allfriend"].isArray()){
            QJsonArray allfriend=jsonobject["allfriend"].toArray();
            qDebug()<<allfriend;
            for(QJsonArray::const_iterator it=allfriend.constBegin();it!=allfriend.constEnd();++it){
                const auto &item =*it;
                if (item.isObject()) {
                    QJsonObject cu_friend = item.toObject();
                    emit createItem(cu_friend);
                }
            }
        }

    }
    else if(type=="get_history_response"||type=="chat_message")
    {
        emit sendToCHAT(jsonobject);
    }

}
