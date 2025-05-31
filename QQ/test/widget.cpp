#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringListModel *model=new QStringListModel(this);
    ui->listView->setModel(model);
    QStringList list={"Item 1", "Item 2", "Item 3"};
    model->setStringList(list);
    ui->listView->setDragEnabled(true);
    ui->listView->setAcceptDrops(true);
    ui->listView->setDropIndicatorShown(true);
    ui->listView->setDefaultDropAction(Qt::MoveAction);
    connect(ui->listView,&QListView::clicked,this,[](const QModelIndex& index){
        qDebug()<<index.data().toString();
    });
}

Widget::~Widget()
{
    delete ui;
}
