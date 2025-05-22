#ifndef INDEX_H
#define INDEX_H
#include <QMainWindow>
#include <QWidget>
#include <QMenu>
#include <QTcpSocket>
#include <QDebug>
namespace Ui {
class Index;
}

class Index : public QMainWindow
{
    Q_OBJECT

public:
    explicit Index(QTcpSocket *s,QWidget *parent = nullptr);
    ~Index();

private:
    Ui::Index *ui;
    QTcpSocket *socket;
    QMenu *menu;
signals:

};

#endif // INDEX_H
