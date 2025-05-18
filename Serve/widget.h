#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include "mythread.h"
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTcpSocket>
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
    void newMessageReciver(QByteArray byte);//处理信息

private:
    Ui::Widget *ui;
    QTcpServer *server;
    QSqlDatabase db;
};
#endif // WIDGET_H
