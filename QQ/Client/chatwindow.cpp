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
    ui->MessageListWidget->setStyleSheet(R"(
    QListWidget {
        background: transparent;
        border: none;
    }
    QListWidget::item {
        background: transparent;
        border: none;
        margin: 0px;
        padding: 0px;
    }
    QListWidget::item:hover {
        background: transparent;
    }
    QListWidget::item:selected {
        background: transparent;
    }
    )");

    this->installEventFilter(new DragEvent());
    this->socket = socket;
    this->selfAccount = selfAccount;
    this->friendAccount = friendAccount;
    this->friendName = friendName;
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

    QString messageLine =  text;
    addMessageToList(messageLine, "皇帝",true);
    ui->EditArea->clear();
}

void ChatWindow::receiveMessage(const QJsonObject &js) {
    qDebug()<<__func__<<js;
    if (js["from"].toString() == friendAccount) {
        QString time = js["time"].toString();
        QString content = js["content"].toString();
        QString messageLine = content;
        addMessageToList(messageLine, js["name"].toString(),false);
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
            QDateTime timestamp = QDateTime::fromString(time, "yyyy-MM-dd HH:mm:ss");
            if(lastMessageTime.isNull()||lastMessageTime.date() != timestamp.date() ||lastMessageTime.secsTo(timestamp) > TIME_THRESHOLD_SECONDS)
            {
                QListWidgetItem* item = new QListWidgetItem(ui->MessageListWidget);
                QWidget *timeWidget = createTimeLabel(time);
                item->setSizeHint(timeWidget->sizeHint());
                ui->MessageListWidget->addItem(item);
                ui->MessageListWidget->setItemWidget(item, timeWidget);
            }
            QString messageLine =   content;
            bool isOwn = (sender == selfAccount);
            addMessageToList(messageLine, name,isOwn);
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

void ChatWindow::addMessageToList(const QString &text, const QString &name, bool isOwnMessage)
{
    // 创建一个新的 QWidget 容器，包含名字标签和消息气泡
    QWidget *container = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setSpacing(2);
    layout->setContentsMargins(10, 0, 10, 0);

    // 名字标签
    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("color: gray; font-size: 10px;");
    if (isOwnMessage) {
        nameLabel->setAlignment(Qt::AlignRight);
    } else {
        nameLabel->setAlignment(Qt::AlignLeft);
    }

    // 消息气泡
    MessageBubbleWidget *bubble = new MessageBubbleWidget(text, isOwnMessage);

    // 添加到布局
    layout->addWidget(nameLabel);
    layout->addWidget(bubble);

    // 设置 QListWidgetItem
    QListWidgetItem *item = new QListWidgetItem(ui->MessageListWidget);
    item->setSizeHint(container->sizeHint());

    ui->MessageListWidget->addItem(item);
    ui->MessageListWidget->setItemWidget(item, container);
    ui->MessageListWidget->scrollToBottom();
}

QWidget* ChatWindow::createTimeLabel(const QString &time)
{
    QLabel *label = new QLabel(time);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: gray; font-size: 12px; padding: 5px;");

    QWidget *wrapper = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(wrapper);
    layout->addWidget(label);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    return wrapper;
}
