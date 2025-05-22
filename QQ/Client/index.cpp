#include "index.h"
#include "ui_index.h"
#include "dragevent.h"
Index::Index(QTcpSocket *s,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Index)
{
    ui->setupUi(this);
    socket=s;
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

}

Index::~Index()
{
    delete ui;
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

