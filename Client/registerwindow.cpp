#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "mainwindow.h"

Register::Register(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint);
    this->installEventFilter(new DragEvent(this));
    connect(ui->Close,&QToolButton::clicked,this,&Register::on_Close_triggered);
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


void Register::on_Close_triggered()
{
    this->close();
}

