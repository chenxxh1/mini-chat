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
namespace Ui {
class FriendManagement;
}

class FriendManagement : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendManagement(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~FriendManagement();

private:
    Ui::FriendManagement *ui;
    QString account;
    QTcpSocket *socket;
    QStandardItemModel *model;
public slots:
    void fromIN(QJsonObject jsonobject);
    void update();
signals:
    void FM_close();
};

#endif // FRIENDMANAGEMENT_H
