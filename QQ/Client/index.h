#ifndef INDEX_H
#define INDEX_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include <addfriend.h>
#include <QJsonObject>
namespace Ui {
class Index;
}

class Index : public QMainWindow
{
    Q_OBJECT

public:
    explicit Index(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~Index();
signals:
    void I_close();
public slots:
    void closeButtonC();
    void addButton();
    void addFriend();
    void createGroup();
    void frommain(QJsonObject jsonobject);//处理来自main的信息
private slots:
    void on_messageButton_clicked();

    void on_friendButton_clicked();

    void on_setButton_clicked();
    void comeback();

private:
    Ui::Index *ui;
    QTcpSocket *socket;
    bool addToolButtonisclicked;
    QPushButton* createGroupButton ;
    QPushButton* addFriendOrGroupButton;
    AddFriend *addF;
    QJsonObject jsonOb;
signals:
    void sendToAF(QJsonObject jsonobject);//发送给AF
};

#endif // INDEX_H
