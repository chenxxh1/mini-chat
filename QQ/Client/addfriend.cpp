#include "addfriend.h"
#include "ui_addfriend.h"
#include "dragevent.h"
AddFriend::AddFriend(QTcpSocket *s,QJsonObject js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    mymodel= new QStringListModel(this);
    socket=s;
    jsonOb=js;
    ui->resultlistView->setModel(mymodel);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->searchlindEdit,&QLineEdit::editingFinished,this,&AddFriend::on_searchlindEdit);
    connect(ui->closepushButton,&QPushButton::clicked,this,[this](){
        emit this->Add_close();
    });
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::on_searchlindEdit()
{
    qDebug()<<__func__;
    QString message=ui->searchlindEdit->text();
    qDebug()<<message;
    QStringList list =mymodel->stringList();
    list.append(message);
    mymodel->setStringList(list);

    QJsonObject object;
    object["message"]=message;//信息
    object["sendaccount"]=jsonOb.value("account").toString();
    object["sendnickname"]=jsonOb.value("nickname").toString();
    object["id"]=jsonOb.value("id").toString();
    object["type"]="addFriend_search";//添加好友搜索

    QByteArray byte =QJsonDocument(object).toJson();
    socket->write(byte);
    //qDebug()<<object;
}

