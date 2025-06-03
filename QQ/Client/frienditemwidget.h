#ifndef FRIENDITEMWIDGET_H
#define FRIENDITEMWIDGET_H

#include <QListView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

class FriendItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FriendItemWidget(const QString& message, int status, QWidget* parent = nullptr);

signals:
    void agreeButtonClicked(const QString& message);
    void refuseButtonClicked(const QString& message);
    void sendMessageButtonClicked(const QString& message);
private:
    QLabel* messageLabel;
    QPushButton* agreeButton;
    QPushButton* refuseButton;
    QPushButton* sendMessageButton;
};

#endif // FRIENDITEMWIDGET_H
