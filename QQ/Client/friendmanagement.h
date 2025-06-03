#ifndef FRIENDMANAGEMENT_H
#define FRIENDMANAGEMENT_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTcpSocket>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QByteArray>
#include <QHBoxLayout>
#include <QPushButton>
namespace Ui {
class FriendManagement;
}

class FriendManagement : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendManagement(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~FriendManagement();
    void addFriend(const QString& account, const QString& nickname, int status);
private:
    Ui::FriendManagement *ui;
    QString account;
    QTcpSocket *socket;
    QStandardItemModel *model;
    void addFriendItem(const QString& message, int status);
public slots:
    void fromIN(QJsonObject jsonobject);
    void update();
    void onAgreeButtonClicked(const QString& message) {
        qDebug() << "同意按钮被点击：" << message;
    }
    void onRefuseButtonClicked(const QString& message) {
        qDebug() << "拒绝按钮被点击：" << message;
    }
    void onSendMessageButtonClicked(const QString& message) {
        qDebug() << "发送信息按钮被点击：" << message;
    }

signals:
    void FM_close();
};

#endif // FRIENDMANAGEMENT_H
