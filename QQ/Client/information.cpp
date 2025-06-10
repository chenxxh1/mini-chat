#include "information.h"
#include "ui_information.h"
#include "dragevent.h"
#include "chatwindow.h"
Information::Information(QTcpSocket *s, QJsonObject v,QString message, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Information)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    socket=s;
    static QRegularExpression re("账号：(\\d+)，昵称：([^，]+)");//使用静态QRegularExpression对象替代临时对象可以提高性能和效率
    QRegularExpressionMatch match = re.match(message);
    viewer=v;
    if (match.hasMatch()) {
        account = match.captured(1); // 获取第一个捕获组的内容（账号）
        nickname = match.captured(2); // 获取第二个捕获组的内容（昵称）
    } else {
        qDebug() << "未找到匹配的内容";
    }
    v_account=viewer["account"].toString();//查看者的账号
    //向服务端发送检查好友信息
    QJsonObject jsonObject;
    jsonObject["type"]="checkFriend";
    jsonObject["v_account"]=v_account;
    jsonObject["account"]=account;
    ui->nicknamelabel->setText(nickname);
    ui->accountlabel->setText(account);
    QByteArray byte=QJsonDocument(jsonObject).toJson();
    socket->write(byte);
    connect(ui->closetoolButton,&QToolButton::clicked,this,[this](){
        this->close();
        emit INF_close();
    });
    connect(ui->sendpushButton,&QPushButton::clicked,this,&Information::sendPushClick);
}

Information::~Information()
{
    delete ui;
}
void Information::fromAD(QJsonObject jsonobject){
    QString type =jsonobject["type"].toString();
    if(type=="checkFriend_response"){
        QString is_friend=jsonobject["result"].toString();
        if(is_friend=="is not friend"){
            ui->sendpushButton->setText("发送好友申请");
        }
    }else if(type=="friend_request_response"){
        QMessageBox::information(this,"提示","已发送申请");
    }else if(type == "get_history_response"||type=="chat_message"){
        emit sendToCHAT(jsonobject);
    }
}
void Information::sendPushClick(){
    if(ui->sendpushButton->text()=="发送好友申请"){
        QString v_account=viewer["account"].toString();//查看者的账号
        //向服务端发送好友申请
        QJsonObject jsonObject;
        jsonObject["type"]="friend_request";
        jsonObject["v_account"]=v_account;
        jsonObject["account"]=account;
        ui->nicknamelabel->setText(nickname);
        ui->accountlabel->setText(account);
        QByteArray byte=QJsonDocument(jsonObject).toJson();

        socket->write(byte);
    }else{
        QString friendName=nickname;
        if(account==v_account){
            friendName="self";
        }
        ChatWindow *chat = new ChatWindow(socket, v_account, account,friendName, nullptr);
        connect(this,&Information::sendToCHAT,chat,&ChatWindow::onReadyRead);
        chat->show();
    }

}


