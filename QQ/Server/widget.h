#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlError>
#include <mythread.h>
#include <QMap>
#include <QCryptographicHash>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    bool isAccountExists(QString account);//查询账号是否存在
    QJsonArray findallfriend(const QString &accounte);
    QJsonArray findallgroup(const QString &account);
    QString crypassword(const QString& password);
    bool insertFriend(const QString &v_account, const QString &account, const int &status);
    void setPrintInterval(const int &seconds);
    void find(const QString &account,QJsonObject &response);
    void sendToClient(QTcpSocket *socket, const QJsonObject &response);
    void printOnlineAccounts();
public slots:
    void newClient();
    void newMessageReciver(QByteArray byte,Mythread *currentThread);//处理信息
    void disClient(QByteArray byte,Mythread *t);


private:
    Ui::Widget *ui;
    QTcpServer *server;
    QSqlDatabase db;
    QMap<QString,Mythread*>threadInfo;
    QMap<QString, QTcpSocket*> userSocketMap;
    QTimer *timer;
};
#endif // WIDGET_H
