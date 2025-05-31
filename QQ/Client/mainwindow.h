#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTcpSocket>//网络
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include "registerwindow.h"
#include "index.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_aggreButton_clicked();

    void on_LogButton_clicked();

    void on_CloseButton_triggered();

    void RegisterButton_clicked();
    void serverSocket();
    void againShow();
private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    Register *r;
    Index *index;
    QJsonObject jsonOb;
};
#endif // MAINWINDOW_H
