#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QMainWindow>
#include <QStringListModel>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardItemModel>
#include <QStandardItem>
#include "information.h"
namespace Ui {
class AddFriend;
}

class AddFriend : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddFriend(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~AddFriend();

public slots:
    void on_searchlindEdit();
    void fromIN(QJsonObject jsonobject);//处理接受的搜索结果信息
    void viewClickedSlots(const QModelIndex& index);
    void Infor_close();
private:
    Ui::AddFriend *ui;
    QTcpSocket *socket;
    QJsonObject jsonOb;
    QStandardItemModel *model;
    Information *info;
signals:
    void Add_close();
    void sendToINF(QJsonObject jsonobject);
};

#endif // ADDFRIEND_H
