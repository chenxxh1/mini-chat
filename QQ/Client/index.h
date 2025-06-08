#ifndef INDEX_H
#define INDEX_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include "addfriend.h"
#include <QJsonObject>
#include <QStandardItem>
#include <QStandardItemModel>
#include "friendmanagement.h"
#include "chatwindow.h"
namespace Ui {
class Index;
}

class Index : public QMainWindow
{
    Q_OBJECT

public:
    explicit Index(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~Index();


public slots:
    void closeButtonC();
    void addButton();
    void addFriend();
    void createGroup();
    void frommain(QJsonObject jsonobject);//处理来自main的信息
    void on_messageButton_clicked();

    void on_friendButton_clicked();

    void on_setButton_clicked();
    void comeback();
    void onAgreeButtonClicked(const QJsonObject &js);
    void onRefuseButtonClicked(const QJsonObject &js);
    void onSendMessageButtonClicked(const QJsonObject &js);

private:
    Ui::Index *ui;
    QTcpSocket *socket;
    bool addToolButtonisclicked;
    bool setButtonclicked;
    QPushButton* createGroupButton ;
    QPushButton* addFriendOrGroupButton;
    AddFriend *addF;//添加好友界面
    QJsonObject jsonOb;
    FriendManagement *fm;//好友管理界面
    QPushButton* changeAccount;//改变账号
    QStandardItemModel *model;
    QStandardItemModel *sendToIndexmodel;
    QMap<QString, ChatWindow*> chatWindowMap;
    QString account;
    QString nickname;
    void addFriendItem(const QJsonObject &js);
signals:
    void sendToAF(QJsonObject jsonobject);//发送给AF
    void sendToFM(QJsonObject jsonobject);
    void I_close();
    void createItem(QJsonObject obj);
    void sendToCHAT(QJsonObject jsonobject);
};

#endif // INDEX_H
