/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QToolButton *CloseButton;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QLabel *imageLable;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QLineEdit *accountEdit;
    QLineEdit *passwordEdit;
    QRadioButton *aggreButton;
    QPushButton *RegisterButton;
    QPushButton *LogButton;
    QSpacerItem *verticalSpacer_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(378, 510);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(centralwidget);
        widget->setObjectName("widget");
        widget->setStyleSheet(QString::fromUtf8("QToolButton{\n"
"	background-color: rgba(0, 0, 0,0);\n"
"    border:none;\n"
"}\n"
"QToolButton#CloseButton{\n"
"	border-image: url(:/imgs/icons8-close.svg);\n"
"}\n"
"QToolButton#CloseButton:hover{\n"
"	background-color: rgb(255, 68, 6);\n"
"	border-image: url(:/imgs/whiteclose.svg);\n"
"}\n"
"QLabel#imageLable{\n"
"	background-color: rgb(255, 255, 255);\n"
"    border-radius:50px;\n"
"}\n"
"QPushButton#LogButton{\n"
"	background-color: rgb(170, 233, 255);\n"
"    border-radius:10px;\n"
"    border:none;\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(widget);
        verticalLayout_3->setObjectName("verticalLayout_3");
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        CloseButton = new QToolButton(widget);
        CloseButton->setObjectName("CloseButton");
        CloseButton->setEnabled(true);
        CloseButton->setMinimumSize(QSize(25, 25));
        CloseButton->setMaximumSize(QSize(25, 25));
        CloseButton->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(CloseButton);


        verticalLayout_3->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(20, 15, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        imageLable = new QLabel(widget);
        imageLable->setObjectName("imageLable");
        imageLable->setMinimumSize(QSize(105, 110));
        imageLable->setMaximumSize(QSize(105, 110));
        imageLable->setPixmap(QPixmap(QString::fromUtf8(":/imgs/icons8-qq.svg")));
        imageLable->setScaledContents(true);
        imageLable->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout->addWidget(imageLable);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 0, 20, -1);
        accountEdit = new QLineEdit(widget);
        accountEdit->setObjectName("accountEdit");
        accountEdit->setMinimumSize(QSize(320, 50));
        accountEdit->setMaximumSize(QSize(320, 50));
        accountEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
        accountEdit->setDragEnabled(false);
        accountEdit->setReadOnly(false);

        verticalLayout->addWidget(accountEdit);

        passwordEdit = new QLineEdit(widget);
        passwordEdit->setObjectName("passwordEdit");
        passwordEdit->setMinimumSize(QSize(320, 50));
        passwordEdit->setMaximumSize(QSize(320, 50));
        passwordEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(passwordEdit);

        aggreButton = new QRadioButton(widget);
        aggreButton->setObjectName("aggreButton");

        verticalLayout->addWidget(aggreButton);

        RegisterButton = new QPushButton(widget);
        RegisterButton->setObjectName("RegisterButton");
        RegisterButton->setMinimumSize(QSize(320, 40));
        RegisterButton->setMaximumSize(QSize(320, 40));

        verticalLayout->addWidget(RegisterButton);

        LogButton = new QPushButton(widget);
        LogButton->setObjectName("LogButton");
        LogButton->setEnabled(false);
        LogButton->setMinimumSize(QSize(320, 40));
        LogButton->setMaximumSize(QSize(320, 40));

        verticalLayout->addWidget(LogButton);


        verticalLayout_3->addLayout(verticalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        verticalLayout_2->addWidget(widget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 378, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        CloseButton->setText(QString());
        imageLable->setText(QString());
        accountEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\264\246\345\217\267", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "\345\257\206\347\240\201", nullptr));
        aggreButton->setText(QCoreApplication::translate("MainWindow", "\345\267\262\351\230\205\350\257\273\345\271\266\345\220\214\346\204\217\346\234\215\345\212\241\345\215\217\350\256\256\345\222\214QQ\351\232\220\347\247\201\346\234\215\345\212\241", nullptr));
        RegisterButton->setText(QCoreApplication::translate("MainWindow", "\346\263\250\345\206\214", nullptr));
        LogButton->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
