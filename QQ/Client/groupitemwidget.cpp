#include "groupitemwidget.h"
#include <QHBoxLayout>
#include <QDebug>

GroupItemWidget::GroupItemWidget(const QJsonObject &groupInfo, QWidget *parent)
    : QWidget(parent), currentGroupInfo(groupInfo)
{
    QString groupName = groupInfo["group_name"].toString();
    QString groupId = groupInfo["group_id"].toString();

    groupNameLabel = new QLabel("群聊名称：" + groupName, this);
    groupIdLabel = new QLabel("群聊号：" + groupId, this);

    enterGroupButton = new QPushButton("进入群聊", this);
    connect(enterGroupButton, &QPushButton::clicked, this, &GroupItemWidget::onEnterGroupClicked);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(groupNameLabel);
    layout->addWidget(groupIdLabel);
    layout->addStretch();
    layout->addWidget(enterGroupButton);

    this->setLayout(layout);
    this->setFixedHeight(40);
}

void GroupItemWidget::onEnterGroupClicked()
{
    qDebug() << __func__ << currentGroupInfo;
    emit enterGroupButtonClicked(currentGroupInfo);
}
