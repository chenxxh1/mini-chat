#include "dragevent.h"

DragEvent::DragEvent(QObject *parent)
    : QObject{parent}
{}
bool DragEvent::eventFilter(QObject* object,QEvent* event){
    auto m =dynamic_cast<QMainWindow*>(object);
    if(!m){
        return false;
    }
    if(event->type()==QEvent::MouseButtonPress){
        auto e =dynamic_cast<QMouseEvent*>(event);
        if(!e)return false;
        mousePoint=e->pos();
    }else if(event->type()==QEvent::MouseMove){
        auto e=dynamic_cast<QMouseEvent*>(event);
        if(!e)return false;
        if(e->buttons()&Qt::MouseButton::LeftButton){
            m->move(e->globalPosition().toPoint()-mousePoint);
        }
    }
    return QObject::eventFilter(object,event);
}
