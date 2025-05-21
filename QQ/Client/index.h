#ifndef INDEX_H
#define INDEX_H

#include <QWidget>

namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:
    explicit Index(QWidget *parent = nullptr);
    ~Index();

private:
    Ui::Index *ui;
};

#endif // INDEX_H
