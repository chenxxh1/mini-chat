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
    explicit FriendItemWidget(const QJsonObject &js, QWidget* parent = nullptr);

signals:
    void agreeButtonClicked(const QJsonObject &js);
    void refuseButtonClicked(const QJsonObject &js);
    void sendMessageButtonClicked(const QJsonObject &js);
    void deleteButtonClicked(const QJsonObject &js);
private:
    QLabel* messageLabel;
    QPushButton* agreeButton;
    QPushButton* refuseButton;
    QPushButton* sendMessageButton;
    QPushButton* deleteButton;
    QJsonObject jsonobject;
};

#endif // FRIENDITEMWIDGET_H
