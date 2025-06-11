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
#include "chatwindow.h"
namespace Ui {
class FriendManagement;
}

class FriendManagement : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendManagement(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~FriendManagement();
    void update();
private:
    Ui::FriendManagement *ui;
    QString account;
    QString nickname;
    QTcpSocket *socket;
    QStandardItemModel *model;
    QStandardItemModel *sendToIndexmodel;
    QMap<QString, ChatWindow*> chatWindowMap;
    void addFriendItem(const QJsonObject &js);

public slots:
    void fromIN(QJsonObject jsonobject);
    void onAgreeButtonClicked(const QJsonObject &js);
    void onRefuseButtonClicked(const QJsonObject &js);
    void onSendMessageButtonClicked(const QJsonObject &js);
    void onDeleteButtonClicked(const QJsonObject &js);
signals:
    void createItem(QJsonObject obj);
    void FM_close();
    void sendToCHAT(QJsonObject jsonobject);
    void createGroupItem(QJsonObject obj);
};

#endif // FRIENDMANAGEMENT_H
