#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QTcpSocket>
#include <QMainWindow>

namespace Ui {
class Register;
}

class Register : public QMainWindow
{
    Q_OBJECT

public:
    explicit Register(QTcpSocket *s,QWidget *parent = nullptr);
    ~Register();

private slots:
    void on_Back_clicked();

    void on_Close_triggered();
    void on_RegisterIn_clicked();

signals:
    void R_close();//窗口r关闭的信号
private:
    Ui::Register *ui;
    QTcpSocket *socket;

};

#endif // REGISTERWINDOW_H
