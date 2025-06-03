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
    update();
    connect(ui->closetoolButton,&QToolButton::clicked,this,[this](){
        this->close();
        emit FM_close();
    });
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
FriendManagement::~FriendManagement()
{
    delete ui;
}

void FriendManagement::addFriend(const QString &account, const QString &nickname, int status)
{
    QString message = QString("账号: %1, 昵称: %2").arg(account, nickname);
    addFriendItem(message, status);
}

void FriendManagement::addFriendItem(const QString &message, int status)
{
    // 创建自定义小部件
    FriendItemWidget* widget = new FriendItemWidget(message, status,this);
    widget->show();
    // 连接按钮点击信号
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
    QString type=jsonobject["type"].toString();
    if (type=="View_friend_relationships_response"){
        if(jsonobject.contains("allfriend")&&jsonobject["allfriend"].isArray()){
            QJsonArray allfriend=jsonobject["allfriend"].toArray();
            for(QJsonArray::const_iterator it=allfriend.constBegin();it!=allfriend.constEnd();++it){
                const auto &item =*it;
                if (item.isObject()) {
                    QJsonObject cu_friend = item.toObject();
                    //qDebug()<<cu_friend;
                    QString account = cu_friend["friend_account"].toString();
                    QString nickname = cu_friend["friend_nickname"].toString();
                    int status=cu_friend["status"].toInt();
                    addFriend(account, nickname, status);
                }
            }
        }
    }

}
