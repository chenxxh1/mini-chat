#include "index.h"
#include "ui_index.h"
#include "dragevent.h"
#include "frienditemwidget.h"
Index::Index(QTcpSocket *s,QJsonObject js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Index)
{
    ui->setupUi(this);
    socket=s;
    jsonOb=js;

    addF=new AddFriend(socket,js,this);
    connect(this,&Index::sendToAF,addF,&AddFriend::fromIN);
    account =js["account"].toString();
    nickname=js["nickname"].toString();
    fm=new FriendManagement(socket,jsonOb,this);
    connect(this,&Index::sendToFM,fm,&FriendManagement::fromIN);
    connect(ui->closeButton,&QToolButton::clicked,this,&Index::closeButtonC);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->smallButon,&QToolButton::clicked,this,[this](){
        this->showMinimized();
    });
    setButtonclicked=false;
    changeAccount = new QPushButton("更改账号",this);
    changeAccount->hide();
    addToolButtonisclicked=false;
    createGroupButton = new QPushButton("创建群聊", this);
    addFriendOrGroupButton = new QPushButton("添加好友/群", this);
    createGroupButton->hide();
    addFriendOrGroupButton->hide();
    connect(ui->addToolButton,&QToolButton::clicked,this,&Index::addButton);
    connect(ui->friendButton,&QPushButton::clicked,this,&Index::on_friendButton_clicked);
    connect(changeAccount,&QPushButton::clicked,this,&Index::closeButtonC);
    connect(addFriendOrGroupButton,&QPushButton::clicked,this,&Index::addFriend);
    connect(createGroupButton,&QPushButton::clicked,this,&Index::createGroup);
    connect(addF,&AddFriend::Add_close,this,&Index::comeback);
    connect(fm,&FriendManagement::FM_close,this,&Index::comeback);
    model=new QStandardItemModel(this);
    connect(this,&Index::createItem,this,&Index::addFriendItem);
    ui->messagelistView->setModel(model);
    // 设置 QListView 的大小调整策略
    ui->messagelistView->setResizeMode(QListView::Adjust);
    ui->messagelistView->setUniformItemSizes(true);
}
void Index::frommain(QJsonObject jsonobject){
    qDebug()<<__func__<<jsonobject;
    QString type=jsonobject["type"].toString();
    if(type=="addFriend_searcher_reponse"
        ||type=="checkFriend_response"
        ||type=="friend_request_response"){
        emit sendToAF(jsonobject);
    }else if(type=="View_friend_relationships_response"){
        model->clear();
        if(jsonobject.contains("allfriend")&&jsonobject["allfriend"].isArray()){
            QJsonArray allfriend=jsonobject["allfriend"].toArray();
            qDebug()<<allfriend;
            for(QJsonArray::const_iterator it=allfriend.constBegin();it!=allfriend.constEnd();++it){
                const auto &item =*it;
                if (item.isObject()) {
                    QJsonObject cu_friend = item.toObject();
                    qDebug()<<cu_friend;
                    int status=cu_friend["status"].toInt();
                    if(!status) continue;
                        emit createItem(cu_friend);
                    }
                }
            }
        emit sendToFM(jsonobject);
    }else if(type=="get_history_response"||type=="chat_message"){
        emit sendToCHAT(jsonobject);
        emit sendToAF(jsonobject);
        emit sendToFM(jsonobject);
    }
}
Index::~Index()
{
    delete ui;
}



void Index::addFriend(){
    this->hide();
    addButton();
    addF->show();
}
void Index::comeback(){
    this->show();
}


void Index::createGroup(){
    qDebug()<<__func__;
}

void Index::closeButtonC(){
    this->close();
    emit I_close();
}
void Index::addButton(){
    if(addToolButtonisclicked){
        createGroupButton->hide();
        addFriendOrGroupButton->hide();
        addToolButtonisclicked=false;
    }else{
        createGroupButton->setFixedSize(120, 30);
        addFriendOrGroupButton->setFixedSize(120, 30);
        QPoint addToolButtonPos =ui->addToolButton->pos();
        QPoint creatPos =addToolButtonPos +QPoint(-100,30);
        QPoint addFriend =addToolButtonPos +QPoint(-100,60);
        createGroupButton->move(creatPos);
        addFriendOrGroupButton->move(addFriend);
        createGroupButton->show();
        addFriendOrGroupButton->show();
        addToolButtonisclicked=true;
    }
}


void Index::on_messageButton_clicked()
{
    qDebug()<<__func__;
}


void Index::on_friendButton_clicked()
{
    qDebug()<<__func__;

    fm->show();
    this->hide();
}


void Index::on_setButton_clicked()
{
    if(setButtonclicked){
        changeAccount->hide();
        setButtonclicked=false;
    }else{
        changeAccount->setFixedSize(120, 30);
        QPoint setButtonPos =ui->setButton->pos();
        QPoint changeAccountPos =setButtonPos +QPoint(80,30);
        changeAccount->move(changeAccountPos);
        changeAccount->show();
        setButtonclicked=true;
    }
}
void Index::onAgreeButtonClicked(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    QJsonObject json=js;
    json["type"]="Agree_the_friend";
    json["account"]=account;
    QByteArray byte =QJsonDocument(json).toJson();
    socket->write(byte);
}

void Index::onRefuseButtonClicked(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    QJsonObject json=js;
    json["type"]="Refuse_the_friend";
    json["account"]=account;
    QByteArray byte =QJsonDocument(json).toJson();
    socket->write(byte);
}

void Index::onSendMessageButtonClicked(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    QString friendAccount = js["friend_account"].toString();
    QString friendName = js["friend_nickname"].toString();
    if(account==friendAccount){
        friendName="self";
    }
    ChatWindow *chat = new ChatWindow(socket, account, friendAccount,friendName, nullptr);
    connect(this,&Index::sendToCHAT,chat,&ChatWindow::onReadyRead);
    chat->show();
}
void Index::addFriendItem(const QJsonObject &js)
{
    qDebug()<<__func__<<js;
    // 创建自定义小部件
    QJsonObject newjs=js;
    newjs["account"]=account;
    newjs["type"]="INDEX";
    FriendItemWidget* widget = new FriendItemWidget(newjs,this);
    widget->show();
    // 连接按钮点击信号
    connect(widget, &FriendItemWidget::agreeButtonClicked, this, &Index::onAgreeButtonClicked);
    connect(widget, &FriendItemWidget::refuseButtonClicked, this, &Index::onRefuseButtonClicked);
    connect(widget, &FriendItemWidget::sendMessageButtonClicked, this, &Index::onSendMessageButtonClicked);

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

