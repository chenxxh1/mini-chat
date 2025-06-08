#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QTcpSocket *socket, const QString &selfAccount, const QString &friendAccount, QWidget *parent = nullptr);
    ~ChatWindow();
    void receiveMessage(const QJsonObject &js);
    void getHistory();

private slots:
    void on_SendButton_clicked();


    void on_close_triggered();
public slots:
    void onReadyRead(QJsonObject jsonobject);
private:
    Ui::ChatWindow *ui;
    QTcpSocket* socket;
    QString selfAccount;
    QString friendAccount;
};

#endif // CHATWINDOW_H
