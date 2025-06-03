#include "frienditemwidget.h"

FriendItemWidget::FriendItemWidget(const QString &message, int status, QWidget *parent): QWidget(parent), messageLabel(new QLabel(message, this)) {
    QHBoxLayout* layout = new QHBoxLayout(this);
    qDebug()<<"messageLabel:"<<message;

    // 设置布局的内边距和控件间距
    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(10);

    // 设置文本标签大小
    messageLabel->setFixedSize(200, 30);
    messageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    layout->addWidget(messageLabel);

    // 添加伸展因子，将按钮推向右侧
    layout->addStretch();

    // 创建按钮
    agreeButton = new QPushButton("同意", this);
    refuseButton = new QPushButton("拒绝", this);
    sendMessageButton = new QPushButton("发送信息", this);

    // 设置按钮大小和大小策略
    agreeButton->setFixedSize(80, 30);
    refuseButton->setFixedSize(80, 30);
    sendMessageButton->setFixedSize(100, 30);
    agreeButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    refuseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sendMessageButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    // 根据状态设置按钮的可见性
    agreeButton->setVisible(status == 0);
    refuseButton->setVisible(status == 0);
    sendMessageButton->setVisible(status == 1);

    // 添加按钮到布局
    layout->addWidget(agreeButton);
    layout->addWidget(refuseButton);
    layout->addWidget(sendMessageButton);

    // 设置布局
    this->setLayout(layout);

    // 连接按钮的点击信号
    connect(agreeButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "同意按钮被点击，消息：" << messageLabel->text();
        emit agreeButtonClicked(messageLabel->text());
    });

    connect(refuseButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "拒绝按钮被点击，消息：" << messageLabel->text();
        emit refuseButtonClicked(messageLabel->text());
    });

    connect(sendMessageButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "发送信息按钮被点击，消息：" << messageLabel->text();
        emit sendMessageButtonClicked(messageLabel->text());
    });
}
