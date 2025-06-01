#include "information.h"
#include "ui_information.h"
#include "dragevent.h"
information::information(QTcpSocket *s, QJsonObject viewer,QString message, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::information)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    socket=s;
    QRegularExpression re("账号：(\\d+)，昵称：([^，]+)");
    QRegularExpressionMatch match = re.match(message);
    QString account;
    QString nickname;
    if (match.hasMatch()) {
        account = match.captured(1); // 获取第一个捕获组的内容（账号）
        nickname = match.captured(2); // 获取第二个捕获组的内容（昵称）
    } else {
        qDebug() << "未找到匹配的内容";
    }
    QString v_account=viewer["account"].toString();//查看者的账号
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
    });
}

information::~information()
{
    delete ui;
}
void information::fromAD(QJsonObject jsonobject){
    QString type =jsonobject["type"].toString();
    if(type=="checkFriend_response"){
        QString is_friend=jsonobject["result"].toString();
        qDebug()<<is_friend;
        if(is_friend=="is not friend"){
            ui->sendpushButton->setText("发送好友申请");
        }
    }
}


