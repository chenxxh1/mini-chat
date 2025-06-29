#include "addfriend.h"
#include "ui_addfriend.h"
#include "dragevent.h"
AddFriend::AddFriend(QTcpSocket *s,QJsonObject js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddFriend)
{
    ui->setupUi(this);
    socket=s;
    jsonOb=js;
    model=new QStandardItemModel(this);
    info=nullptr;
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->searchlindEdit,&QLineEdit::editingFinished,this,&AddFriend::on_searchlindEdit);
    connect(ui->closepushButton,&QPushButton::clicked,this,[this](){
        this->close();
        emit this->Add_close();
    });
    connect(ui->resultlistView,&QListView::clicked,this,&AddFriend::viewClickedSlots);
}
void AddFriend::viewClickedSlots(const QModelIndex& index){
    QString message=index.data().toString();
    delete info;
    info=new Information(socket,jsonOb,message,this);
    connect(this,&AddFriend::sendToINF,info,&Information::fromAD);
    connect(info,&Information::INF_close,this,&AddFriend::Infor_close);
    info->show();
    this->close();
}
AddFriend::~AddFriend()
{
    delete ui;
}
void AddFriend::Infor_close(){
    this->show();
}
void AddFriend::fromIN(QJsonObject jsonobject){
    QString type=jsonobject["type"].toString();
    if(type=="addFriend_searcher_reponse"){
        model->clear();
        if(jsonobject.contains("users")&&jsonobject["users"].isArray()){
            QJsonArray users=jsonobject["users"].toArray();
            for(QJsonArray::const_iterator it=users.constBegin();it!=users.constEnd();++it){
                const auto &item =*it;
                if (item.isObject()) {
                    QJsonObject userObject = item.toObject();
                    QString account = userObject["account"].toString();
                    QString nickname = userObject["nickname"].toString();
                    QString message=QString("账号：%1，昵称：%2").arg(account,nickname);
                    QStandardItem *item =new QStandardItem(message);
                    model->appendRow(item);
                }
            }
        }
        ui->resultlistView->setModel(model);
    }
    else if(type=="checkFriend_response"
               ||type=="friend_request_response"
               ||type == "get_history_response"
               ||type=="chat_message"){
        emit sendToINF(jsonobject);
        qDebug()<<"sendToINF";
    }
}
void AddFriend::on_searchlindEdit()
{
    QString message=ui->searchlindEdit->text();
    QJsonObject object;
    object["message"]=message;//信息
    object["sendaccount"]=jsonOb.value("account").toString();
    object["sendnickname"]=jsonOb.value("nickname").toString();
    object["id"]=jsonOb.value("id").toString();
    object["type"]="addFriend_search";//添加好友搜索

    QByteArray byte =QJsonDocument(object).toJson();
    socket->write(byte);
}

