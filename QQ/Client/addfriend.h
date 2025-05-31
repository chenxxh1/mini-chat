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
    void searchSlots(QJsonObject jsonobject);//处理接受的搜索结果信息
    void viewClickedSlots(const QModelIndex& index);
private:
    Ui::AddFriend *ui;
    QTcpSocket *socket;
    QJsonObject jsonOb;
    QStandardItemModel *model;
signals:
    void Add_close();
};

#endif // ADDFRIEND_H
