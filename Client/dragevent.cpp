#include "dragevent.h"

DragEvent::DragEvent(QObject *parent)
    : QObject{parent}
{}
bool DragEvent::eventFilter(QObject* object,QEvent* event){
    auto m =dynamic_cast<QMainWindow*>(object);
    if(!m){
        return false;
    }//判断是否为主窗口，若不是主窗口则不进行处理
    if(event->type()==QEvent::MouseButtonPress){
        auto e =dynamic_cast<QMouseEvent*>(event);
        if(!e)return false;
        mousePoint=e->pos();//记录点击时鼠标的位置
    }else if(event->type()==QEvent::MouseMove){
        auto e=dynamic_cast<QMouseEvent*>(event);
        if(!e)return false;
        if(e->buttons()&Qt::MouseButton::LeftButton){
            m->move(e->globalPosition().toPoint()-mousePoint);//实现窗口的移动，并保证窗口与鼠标的相对位置不变
        }
    }
    return QObject::eventFilter(object,event);
}
