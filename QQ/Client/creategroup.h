#ifndef CREATEGROUP_H
#define CREATEGROUP_H

#include <QMainWindow>
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
class CreateGroup;
}

class CreateGroup : public QMainWindow
{
    Q_OBJECT

public:
    explicit CreateGroup(QTcpSocket *s,QJsonObject js,QWidget *parent = nullptr);
    ~CreateGroup();

public slots:
    void fromIN(QJsonObject jsonobject);//处理接受的搜索结果信息
private:
    Ui::CreateGroup *ui;
    QTcpSocket *socket;
    QJsonObject jsonOb;
    Information *info;
signals:
    void CG_close();
    void sendToINF(QJsonObject jsonobject);
private slots:
    void on_Create_Button_clicked();
};

#endif // CREATEGROUP_H
