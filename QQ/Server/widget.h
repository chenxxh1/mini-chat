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
#include <QSqlError>
#include <mythread.h>
#include <QMap>
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
public slots:
    void newClient();
    void newMessageReciver(QByteArray byte,Mythread *currentThread);//处理信息
    void disClient(QByteArray byte,Mythread *t);

private slots:
    void on_accountpushButton_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QSqlDatabase db;
    QMap<QString,Mythread*>threadInfo;
};
#endif // WIDGET_H
