#include "MessageBubbleWidget.h"

MessageBubbleWidget::MessageBubbleWidget(const QString &text, bool isOwnMessage, QWidget *parent)
    : QWidget(parent)
{
    setupUi(text, isOwnMessage);
}

void MessageBubbleWidget::setupUi(const QString &text, bool isOwnMessage)
{
    // 创建主布局
    auto *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(10);

    // 创建消息标签
    QLabel *messageLabel = new QLabel(text);
    // 设置文本样式
    QFont font;
    font.setPointSize(10);              // 字号
    font.setFamily("Microsoft YaHei");  // 字体（可换成任意常见字体）
    messageLabel->setFont(font);
    messageLabel->setStyleSheet("color: #1b1b1b;"); // 字体颜色（也可以是灰色等）
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageLabel->setContentsMargins(10, 6, 10, 6);
    messageLabel->setMaximumWidth(350);
    messageLabel->setStyleSheet("font-size: 14px;");

    // 气泡背景控件
    QWidget *bubble = new QWidget;
    QVBoxLayout *bubbleLayout = new QVBoxLayout(bubble);
    bubbleLayout->addWidget(messageLabel);
    bubbleLayout->setContentsMargins(0, 0, 0, 0);

    bubble->setLayout(bubbleLayout);
    bubble->setStyleSheet(isOwnMessage
                              ? "background-color: #d4f4dd; border-radius: 12px;"  //  自己的消息为浅绿色
                              : "background-color: #fddde6; border-radius: 12px;"); //  对方消息为浅粉色
    bubble->setFixedWidth(320);

    // 气泡左右对齐
    if (isOwnMessage) {
        mainLayout->addStretch();
        mainLayout->addWidget(bubble);
    } else {
        mainLayout->addWidget(bubble);
        mainLayout->addStretch();
    }

    setLayout(mainLayout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}
