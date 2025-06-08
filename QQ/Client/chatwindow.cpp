#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "dragevent.h"
ChatWindow::ChatWindow(QTcpSocket *socket, const QString &selfAccount, const QString &friendAccount, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    this->socket = socket;
    this->selfAccount = selfAccount;
    this->friendAccount = friendAccount;
    qDebug() << "to_user:::" << selfAccount << friendAccount;
    getHistory();
    connect(ui->close,&QToolButton::clicked,this,&ChatWindow::on_close_triggered);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_SendButton_clicked()
{
    QString text = ui->EditArea->text();
    if (text.isEmpty()) return;

    QJsonObject object;
    object["type"] = "chat_message";
    object["from"] = this->selfAccount;
    object["to"] = this->friendAccount;

    object["content"] = text;
    object["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QByteArray data = QJsonDocument(object).toJson();
    socket->write(data);

    ui->InformationArea->append("我: " + text);
    ui->EditArea->clear();
}

void ChatWindow::receiveMessage(const QJsonObject &js) {
    qDebug()<<__func__<<js;
    if (js["from"].toString() == friendAccount) {
       ui->InformationArea->append(friendAccount + ": " + js["content"].toString());
    }
}

void ChatWindow::onReadyRead(QJsonObject jsonobject)
{
    qDebug()<<__func__<<jsonobject;
    qDebug() << "Data arrived";

    QJsonObject obj = jsonobject;
    qDebug() << "JSON type:" << obj["type"].toString();          
    QString type = obj["type"].toString();
    if (type == "chat_message") {
        receiveMessage(obj);
    }
    else if (type == "get_history_response") {
        QJsonArray history = obj["messages"].toArray();
        for (const QJsonValue &val : history) {
            QJsonObject msg = val.toObject();
            QString sender = msg["from"].toString();
            QString content = msg["content"].toString();
            QString time = msg["time"].toString();
            QString name = msg["name"].toString();
            QString messageLine = "[" + time + "] " + sender + name + ": " + content;
            ui->InformationArea->append(messageLine); // or add time if needed
        }
    }

}


void ChatWindow::on_close_triggered()
{
    this->hide();
}

void ChatWindow::getHistory()
{
    QJsonObject json;
    json["type"] = "get_history";
    json["from"] = this->selfAccount;
    json["to"] = this->friendAccount;
    QByteArray data = QJsonDocument(json).toJson();
    socket->write(data);
}
