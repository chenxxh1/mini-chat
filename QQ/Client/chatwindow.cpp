#include "chatwindow.h"
#include "ui_chatwindow.h"
#include "dragevent.h"
#include <QFile>
#include <QDir>
#include <QDesktopServices>
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
    ui->EditArea->installEventFilter(this);
    this->installEventFilter(new DragEvent());
    this->socket = socket;
    this->selfAccount = selfAccount;
    this->friendAccount = friendAccount;
    this->friendName = friendName;
    if(selfAccount!=friendName){
        ui->FriendName->setText(friendName);
    }
    qDebug() << "to_user:::" << selfAccount << friendAccount;
    loadHistoryFromLocal();
    getHistory();

    connect(ui->close,&QToolButton::clicked,this,&ChatWindow::on_close_triggered);
    connect(ui->SendButton, &QPushButton::clicked, this, &ChatWindow::on_SendButton_clicked);
}
bool ChatWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->EditArea && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if ((keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) &&
            keyEvent->modifiers() == Qt::NoModifier) {
            emit on_SendButton_clicked();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
    ui->EditArea->setFocus();
}

ChatWindow::~ChatWindow()
{
    delete ui;
}



void ChatWindow::on_SendButton_clicked()
{
    QString text = ui->EditArea->toPlainText();
    while (!text.isEmpty() && text.endsWith('\n')) {
        text = text.chopped(1);
    }
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
    saveMessageToLocal(object);
}

void ChatWindow::receiveMessage(const QJsonObject &js) {
    qDebug()<<__func__<<js;
    if (js["from"].toString() == friendAccount) {
        QString time = js["time"].toString();
        QString content = js["content"].toString();
        QString messageLine = content;
        addMessageToList(messageLine, js["name"].toString(),false);
        saveMessageToLocal(js);
    }
}

void ChatWindow::onReadyRead(QJsonObject jsonobject)
{
    qDebug() << __func__ << jsonobject;
    qDebug() << "Data arrived";

    QString type = jsonobject["type"].toString();
    if (type == "chat_message") {
        receiveMessage(jsonobject);
    }
    else if (type == "get_history_response") {
        QJsonArray history = jsonobject["messages"].toArray();
        for (const QJsonValue &val : history) {
            QJsonObject msg = val.toObject();
            processNewMessage(msg);
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

    QWidget *container = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setSpacing(2);
    layout->setContentsMargins(10, 0, 10, 0);


    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("color: gray; font-size: 10px;");
    if (isOwnMessage) {
        nameLabel->setAlignment(Qt::AlignRight);
    } else {
        nameLabel->setAlignment(Qt::AlignLeft);
    }


    MessageBubbleWidget *bubble = new MessageBubbleWidget(text, isOwnMessage);


    layout->addWidget(nameLabel);
    layout->addWidget(bubble);


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

QString ChatWindow::getHistoryFilePath() const {
    QString filename = QString("%1_%2.json").arg(selfAccount).arg(friendAccount);


    QString currentPath = QDir::currentPath();
    QString dirPath = currentPath + "/chat_history";
    QDir dir;


    if (!dir.exists(dirPath)) {
        qDebug() << "正在创建目录：" << dirPath;
        if (!dir.mkpath(dirPath)) {
            qDebug() << "创建目录失败！";
        }
    }

    QString fullPath = dirPath + "/" + filename;
    qDebug() << "create" << fullPath;
    return fullPath;
}

void ChatWindow::saveMessageToLocal(const QJsonObject &msg) {
    QString filePath = getHistoryFilePath();
    qDebug() << "save" << filePath;

    QFile file(filePath);
    QJsonArray historyArray;


    if (file.exists()) {
        qDebug() << "文件已存在，尝试读取历史记录...";
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isArray()) {
                historyArray = doc.array();
            }
            file.close();
        }
    }


    QString messageId = msg["id"].toString();
    if (messageId.isEmpty()) {
        QString content = msg["content"].toString();
        QString time = msg["time"].toString();
        messageId = QString("%1_%2").arg(time).arg(content);
    }

    bool exists = false;
    for (const QJsonValue &val : historyArray) {
        QJsonObject existingMsg = val.toObject();
        QString existingId = existingMsg["id"].toString();
        if (existingId.isEmpty()) {
            QString existingContent = existingMsg["content"].toString();
            QString existingTime = existingMsg["time"].toString();
            existingId = QString("%1_%2").arg(existingTime).arg(existingContent);
        }
        if (existingId == messageId) {
            exists = true;
            break;
        }
    }

    if (!exists) {
        historyArray.append(msg);

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QJsonDocument doc(historyArray);
            qint64 bytesWritten = file.write(doc.toJson());
            qDebug() << " 写入文件成功，共写入" << bytesWritten << "字节";
            file.close();
        } else {
            qDebug() << " 无法打开文件进行写入：" << file.errorString();
        }
    } else {
        qDebug() << " 消息已存在，未写入文件：" << messageId;
    }

}
void ChatWindow::loadHistoryFromLocal() {
    QString filePath = getHistoryFilePath();
    qDebug() << "get" << filePath;

    QFile file(filePath);
    if (!file.exists()) return;

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            QJsonArray history = doc.array();
            for (const QJsonValue &val : history) {
                QJsonObject msg = val.toObject();
                processNewMessage(msg);
            }
        }
        file.close();
    }
}

void ChatWindow::processNewMessage(const QJsonObject &msg) {
    QString messageId = msg["id"].toString();
    if (messageId.isEmpty()) {

        QString content = msg["content"].toString();
        QString time = msg["time"].toString();
        messageId = QString("%1_%2").arg(time).arg(content);
    }

    if (!messageCache.contains(messageId)) {
        QString sender = msg["from"].toString();
        QString content = msg["content"].toString();
        QString name = msg["name"].toString();
        bool isOwn = (sender == selfAccount);

        QDateTime timestamp = QDateTime::fromString(msg["time"].toString(), "yyyy-MM-dd HH:mm:ss");


        if (lastMessageTime.isNull() ||
            lastMessageTime.date() != timestamp.date() ||
            lastMessageTime.secsTo(timestamp) > TIME_THRESHOLD_SECONDS) {
            QListWidgetItem* item = new QListWidgetItem(ui->MessageListWidget);
            QWidget *timeWidget = createTimeLabel(timestamp.toString("yyyy-MM-dd HH:mm:ss"));
            item->setSizeHint(timeWidget->sizeHint());
            ui->MessageListWidget->addItem(item);
            ui->MessageListWidget->setItemWidget(item, timeWidget);
        }


        addMessageToList(content, name, isOwn);


        saveMessageToLocal(msg);


        messageCache.insert(messageId);


        lastMessageTime = timestamp;
    } else {
        qDebug() << " 消息已存在，跳过：" << messageId;
    }
}
