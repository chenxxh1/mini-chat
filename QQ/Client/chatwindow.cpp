#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "dragevent.h"
ChatWindow::ChatWindow(QTcpSocket *socket, const QString &selfAccount, const QString &friendAccount, const QString &friendName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);

    ui->MessageListWidget->setSpacing(5);
    ui->MessageListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->MessageListWidget->setFocusPolicy(Qt::NoFocus);
    ui->MessageListWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);


    this->installEventFilter(new DragEvent());
    this->socket = socket;
    this->selfAccount = selfAccount;
    this->friendAccount = friendAccount;
    if(selfAccount!=friendName){
        ui->FriendName->setText(friendName);
    }
    qDebug() << "to_user:::" << selfAccount << friendAccount;

    getHistory();

    connect(ui->close,&QToolButton::clicked,this,&ChatWindow::on_close_triggered);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::run()
{

}

void ChatWindow::on_SendButton_clicked()
{
    QString text = ui->EditArea->toPlainText();
    if (text.isEmpty()) return;

    QJsonObject object;
    object["type"] = "chat_message";
    object["from"] = this->selfAccount;
    object["to"] = this->friendAccount;

    object["content"] = text;
    object["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QByteArray data = QJsonDocument(object).toJson();
    socket->write(data);

    QString messageLine = "[" + object["time"].toString() + "] 我: " + text;
    addMessageToList(messageLine, true);
    ui->EditArea->clear();
}

void ChatWindow::receiveMessage(const QJsonObject &js) {
    qDebug()<<__func__<<js;
    if (js["from"].toString() == friendAccount) {
        QString time = js["time"].toString();
        QString content = js["content"].toString();
        QString messageLine = "[" + time + "] " + js["name"].toString() + ": " + content;
        addMessageToList(messageLine, false);
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
            QString displayMsg;

            QString messageLine = "[" + time + "] " + name + ": " + content;
            bool isOwn = (sender == selfAccount);
            addMessageToList(messageLine, isOwn);
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

void ChatWindow::addMessageToList(const QString &text, bool isOwnMessage)
{
    QListWidgetItem *item = new QListWidgetItem(ui->MessageListWidget);
    MessageBubbleWidget *bubble = new MessageBubbleWidget(text, isOwnMessage);
    item->setSizeHint(bubble->sizeHint());
    ui->MessageListWidget->addItem(item);
    ui->MessageListWidget->setItemWidget(item, bubble);
    ui->MessageListWidget->scrollToBottom();
}
