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
    void mainsendToAFS(QJsonObject jsonobject);//处理Main发送给AFS的信息，AFS为addFriend的search
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
    void sendToAFS(QJsonObject jsonobject);//发送给AFS
};

#endif // INDEX_H
