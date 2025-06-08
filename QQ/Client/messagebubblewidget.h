#ifndef MESSAGEBUBBLEWIDGET_H
#define MESSAGEBUBBLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class MessageBubbleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageBubbleWidget(const QString &text, bool isOwnMessage, QWidget *parent = nullptr);

private:
    void setupUi(const QString &text, bool isOwnMessage);
};

#endif // MESSAGEBUBBLEWIDGET_H
