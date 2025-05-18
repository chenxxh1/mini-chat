#ifndef DRAGEVENT_H
#define DRAGEVENT_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QMainWindow>
class DragEvent : public QObject
{
    Q_OBJECT
public:
    explicit DragEvent(QObject *parent = nullptr);
    bool eventFilter(QObject* object,QEvent* event);
signals:
private:
    QPoint mousePoint;
};

#endif // DRAGEVENT_H
