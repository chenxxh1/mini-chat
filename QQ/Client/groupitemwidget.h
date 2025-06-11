#ifndef GROUPITEMWIDGET_H
#define GROUPITEMWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QJsonObject>

class GroupItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupItemWidget(const QJsonObject &groupInfo, QWidget *parent = nullptr);

signals:
    void enterGroupButtonClicked(const QJsonObject &groupInfo);

private slots:
    void onEnterGroupClicked();

private:
    QLabel *groupNameLabel;
    QLabel *groupIdLabel;
    QPushButton *enterGroupButton;

    QJsonObject currentGroupInfo;
};

#endif // GROUPITEMWIDGET_H
