#ifndef INFORMATION_H
#define INFORMATION_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QByteArray>
namespace Ui {
class information;
}

class information : public QMainWindow
{
    Q_OBJECT

public:
    explicit information(QTcpSocket *s,QJsonObject viewer,QString a,QWidget *parent = nullptr);
    ~information();//a指要显示的账号,viewer是查看者的信息

private:
    Ui::information *ui;
    QTcpSocket *socket;
    QString message;
public slots:
    void fromAD(QJsonObject jsonobject);
private slots:
};

#endif // INFORMATION_H
