#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "dragevent.h"
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
    void on_aggreButton_clicked();//处理协议与登录间的关系

    void on_LogButton_clicked();//处理登录按钮

    void on_CloseButton_triggered();//处理关闭按钮

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
