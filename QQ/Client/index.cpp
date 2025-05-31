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
    connect(ui->closeButton,&QToolButton::clicked,this,&Index::closeButtonC);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->smallButon,&QToolButton::clicked,this,[this](){
        this->showMinimized();
    });

    addToolButtonisclicked=false;
    createGroupButton = new QPushButton("创建群聊", this);
    addFriendOrGroupButton = new QPushButton("添加好友/群", this);
    createGroupButton->hide();
    addFriendOrGroupButton->hide();
    connect(ui->addToolButton,&QToolButton::clicked,this,&Index::addButton);
    connect(addFriendOrGroupButton,&QPushButton::clicked,this,&Index::addFriend);
    connect(createGroupButton,&QPushButton::clicked,this,&Index::createGroup);
    connect(addF,&AddFriend::Add_close,this,&Index::comeback);
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
    addF->close();
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
}


void Index::on_setButton_clicked()
{
    qDebug()<<__func__;
}

