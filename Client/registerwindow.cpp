#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "mainwindow.h"

Register::Register(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::on_Back_clicked()
{
    MainWindow* m = new MainWindow;
    m->show();
    this->hide();
}

