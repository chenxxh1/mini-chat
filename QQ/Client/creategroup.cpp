#include "creategroup.h"
#include "ui_creategroup.h"
#include "dragevent.h"

CreateGroup::CreateGroup(QTcpSocket *s,QJsonObject js,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CreateGroup)
{
    ui->setupUi(this);
    socket=s;
    jsonOb=js;
    info=nullptr;
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
}

CreateGroup::~CreateGroup()
{
    delete ui;
}

void CreateGroup::fromIN(QJsonObject jsonobject){
    qDebug()<<__func__<<jsonobject;
    QString type=jsonobject["type"].toString();
    if(type=="create_group_response"){
        if(jsonobject["status"].toString()=="success")
        {
            QString groupName = jsonobject["group_name"].toString();
            QString groupId = jsonobject["group_id"].toString();  // 服务器返回的群号

            QMessageBox::information(this, tr("群聊创建成功"),
                                     tr("群聊“%1”创建成功！\n群号为：%2").arg(groupName, groupId));
            this->close();
            emit this->CG_close();
        }else {
            QMessageBox::warning(this, tr("群聊创建失败"), tr("服务器返回失败，请稍后重试。"));
        }

    }
    else if(type=="checkFriend_response"
             ||type=="friend_request_response"
             ||type == "get_history_response"
             ||type=="chat_message"
             ||type=="addFriend_searcher_reponse"){
        emit sendToINF(jsonobject);
        qDebug()<<"sendToINF";
    }
}

void CreateGroup::on_Create_Button_clicked()
{
    QString groupName = ui->GroupName->text();

    // 2. 构造 JSON 请求发送给服务器
    QJsonObject json;
    json["type"] = "create_group";
    json["group_name"] = groupName;
    json["owner_account"] = jsonOb["account"].toString();  // 当前用户账号作为群主

    // 3. 发送给服务器
    QByteArray byte = QJsonDocument(json).toJson();
    socket->write(byte);

    // 4. 可以选择等待服务器反馈（这里简化处理）
    qDebug() << "创建群聊请求已发送：" << groupName;
}

