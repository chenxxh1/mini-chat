#ifndef INDEX_H
#define INDEX_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
namespace Ui {
class Index;
}

class Index : public QMainWindow
{
    Q_OBJECT

public:
    explicit Index(QTcpSocket *s,QWidget *parent = nullptr);
    ~Index();
signals:
    void I_close();
public slots:
    void closeButtonC();
    void addButton();

private:
    Ui::Index *ui;
    QTcpSocket *socket;
    bool addToolButtonisclicked;
    QPushButton* createGroupButton ;
    QPushButton* addFriendOrGroupButton;
};

#endif // INDEX_H
