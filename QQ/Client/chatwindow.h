#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QThread>
#include <QKeyEvent>
#include "messagebubblewidget.h"
const int TIME_THRESHOLD_SECONDS = 300;
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QTcpSocket *socket, const QString &selfAccount,const QString &friendAccount, const QString &friendName, QWidget *parent = nullptr);
    ~ChatWindow();
    void receiveMessage(const QJsonObject &js);
    void getHistory();
    void addMessageToList(const QString &text, const QString &name, bool isOwnMessage);
    QWidget* createTimeLabel(const QString &time);
    QString getHistoryFilePath() const;
    void saveMessageToLocal(const QJsonObject &msg);
    void loadHistoryFromLocal();
private slots:
    void on_SendButton_clicked();


    void on_close_triggered();
signals:
    void enterPressed();
public slots:
    void onReadyRead(QJsonObject jsonobject);
private:
    Ui::ChatWindow *ui;
    QTcpSocket* socket;
    QString selfAccount;
    QString friendAccount;
    QDateTime lastMessageTime;
    QString friendName;
    QSet<QString> messageCache;  // 缓存已处理的消息ID
    bool eventFilter(QObject *obj, QEvent *event);

    void processNewMessage(const QJsonObject &msg);  // 新增处理单条消息的函数
};

#endif // CHATWINDOW_H
