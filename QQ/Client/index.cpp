#include "index.h"
#include "ui_index.h"

Index::Index(QTcpSocket *s,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Index)
{
    ui->setupUi(this);

    socket=s;
    qDebug()<<"Index的构造";
    // 创建菜单
    // QMenu *menu = new QMenu(this);

    // // 创建动作
    // QAction *createGroupChatAction = new QAction("创建群聊", this);
    // QAction *addFriendAction = new QAction("加好友/群", this);

    // // 将动作添加到菜单中
    // menu->addAction(createGroupChatAction);
    // menu->addAction(addFriendAction);

    // // 将菜单关联到按钮的点击事件
    // connect(ui->addpushButton, &QPushButton::clicked, this, [this]() {
    //     this->menu->exec();
    // });
}

Index::~Index()
{
    delete ui;
    delete socket;
    qDebug()<<"Index销毁";
}
