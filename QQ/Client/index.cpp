#include "index.h"
#include "ui_index.h"

Index::Index(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Index)
{
    ui->setupUi(this);
}

Index::~Index()
{
    delete ui;
}
