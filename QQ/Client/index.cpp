#include "index.h"
#include "ui_index.h"
#include "dragevent.h"
Index::Index(QTcpSocket *s,QJsonObject js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Index)
{
    ui->setupUi(this);
    socket=s;
    jsonOb=js;

    addF=new AddFriend(socket,js,this);
    connect(this,&Index::sendToAF,addF,&AddFriend::fromIN);

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
}
void Index::frommain(QJsonObject jsonobject){
    qDebug()<<__func__<<jsonobject;
    QString type=jsonobject["type"].toString();
    if(type=="addFriend_searcher_reponse"
        ||type=="checkFriend_response"
        ||type=="friend_request_response"){
        emit sendToAF(jsonobject);
    }else if(type=="View_friend_relationships_response"||type == "get_history_response"){
        emit sendToFM(jsonobject);
    }

}
Index::~Index()
{
    delete ui;
}
void Index::addFriend(){
    this->hide();
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

