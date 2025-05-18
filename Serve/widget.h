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
public slots:
    void newClient();
    void newMessageReciver(QByteArray byte,QTcpSocket *socket);//处理信息
    void disClient(QByteArray byte);

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QSqlDatabase db;
};
#endif // WIDGET_H
