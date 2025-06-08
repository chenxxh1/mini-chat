#include "frienditemwidget.h"

FriendItemWidget::FriendItemWidget(const QJsonObject &js, QWidget *parent): QWidget(parent) {
    qDebug()<<__func__<<js;
    jsonobject=js;
    QString account=js["friend_account"].toString();
    QString nickname=js["friend_nickname"].toString();
    QString v_account=js["account"].toString();
    if(v_account==account){
        nickname="self";
    }
    int status=js["status"].toInt();
    QString message = QString("账号: %1, 昵称: %2").arg(account, nickname);
    messageLabel=new QLabel(message);
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
        emit agreeButtonClicked(jsonobject);
    });

    connect(refuseButton, &QPushButton::clicked, this, [this]() {
        emit refuseButtonClicked(jsonobject);
    });

    connect(sendMessageButton, &QPushButton::clicked, this, [this]() {
        emit sendMessageButtonClicked(jsonobject);
    });
}
