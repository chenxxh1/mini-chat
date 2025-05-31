#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QMainWindow>
#include <QStringListModel>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
namespace Ui {
class AddFriend;
}

class AddFriend : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddFriend(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~AddFriend();

private slots:
    void on_searchlindEdit();

private:
    Ui::AddFriend *ui;
    QStringListModel *mymodel;
    QTcpSocket *socket;
    QJsonObject jsonOb;
signals:
    void Add_close();
};

#endif // ADDFRIEND_H
