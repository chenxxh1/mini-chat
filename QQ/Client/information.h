#ifndef INFORMATION_H
#define INFORMATION_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QByteArray>
#include <QMessageBox>
namespace Ui {
class Information;
}

class Information : public QMainWindow
{
    Q_OBJECT

public:
    explicit Information(QTcpSocket *s,QJsonObject viewer,QString a,QWidget *parent = nullptr);
    ~Information();//a指要显示的账号,viewer是查看者的信息

private:
    Ui::Information *ui;
    QTcpSocket *socket;
    QString account;
    QString nickname;
    QJsonObject viewer;
    QString v_account;
public slots:
    void fromAD(QJsonObject jsonobject);
    void sendPushClick();
signals:
    void sendToCHAT(QJsonObject jsonobject);
    void INF_close();
};

#endif // INFORMATION_H
