/********************************************************************************
** Form generated from reading UI file 'registerwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERWINDOW_H
#define UI_REGISTERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Register
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *Close;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_7;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *PassWord;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *RegisterIn;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *Back;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *Name;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *ConfirmWord;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_8;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Register)
    {
        if (Register->objectName().isEmpty())
            Register->setObjectName("Register");
        Register->resize(556, 443);
        centralwidget = new QWidget(Register);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 0, 541, 51));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        Close = new QToolButton(layoutWidget);
        Close->setObjectName("Close");
        Close->setMinimumSize(QSize(40, 40));
        Close->setMaximumSize(QSize(40, 40));

        horizontalLayout_5->addWidget(Close);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(0, 50, 541, 321));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_7 = new QSpacerItem(58, 268, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName("label_3");
        label_3->setEnabled(true);
        label_3->setMinimumSize(QSize(80, 20));
        label_3->setMaximumSize(QSize(80, 20));

        horizontalLayout_3->addWidget(label_3);

        PassWord = new QLineEdit(layoutWidget1);
        PassWord->setObjectName("PassWord");
        PassWord->setMinimumSize(QSize(200, 20));
        PassWord->setMaximumSize(QSize(200, 20));

        horizontalLayout_3->addWidget(PassWord);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        gridLayout->addLayout(horizontalLayout_3, 3, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        RegisterIn = new QPushButton(layoutWidget1);
        RegisterIn->setObjectName("RegisterIn");
        RegisterIn->setMinimumSize(QSize(50, 20));
        RegisterIn->setMaximumSize(QSize(50, 20));

        horizontalLayout_4->addWidget(RegisterIn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);

        Back = new QPushButton(layoutWidget1);
        Back->setObjectName("Back");
        Back->setMinimumSize(QSize(50, 20));
        Back->setMaximumSize(QSize(50, 20));

        horizontalLayout_4->addWidget(Back);


        gridLayout->addLayout(horizontalLayout_4, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(layoutWidget1);
        label->setObjectName("label");
        label->setMinimumSize(QSize(80, 20));
        label->setMaximumSize(QSize(80, 20));

        horizontalLayout->addWidget(label);

        Name = new QLineEdit(layoutWidget1);
        Name->setObjectName("Name");
        Name->setMinimumSize(QSize(120, 20));
        Name->setMaximumSize(QSize(120, 20));

        horizontalLayout->addWidget(Name);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName("label_2");
        label_2->setMinimumSize(QSize(80, 20));
        label_2->setMaximumSize(QSize(80, 20));

        horizontalLayout_2->addWidget(label_2);

        ConfirmWord = new QLineEdit(layoutWidget1);
        ConfirmWord->setObjectName("ConfirmWord");
        ConfirmWord->setMinimumSize(QSize(200, 20));
        ConfirmWord->setMaximumSize(QSize(200, 20));

        horizontalLayout_2->addWidget(ConfirmWord);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_2, 2, 0, 1, 1);


        horizontalLayout_6->addLayout(gridLayout);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_8);

        Register->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Register);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 556, 25));
        menu = new QMenu(menubar);
        menu->setObjectName("menu");
        Register->setMenuBar(menubar);
        statusbar = new QStatusBar(Register);
        statusbar->setObjectName("statusbar");
        Register->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());

        retranslateUi(Register);

        QMetaObject::connectSlotsByName(Register);
    } // setupUi

    void retranslateUi(QMainWindow *Register)
    {
        Register->setWindowTitle(QCoreApplication::translate("Register", "MainWindow", nullptr));
        Close->setText(QCoreApplication::translate("Register", "X", nullptr));
        label_3->setText(QCoreApplication::translate("Register", "\351\207\215\346\226\260\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        RegisterIn->setText(QCoreApplication::translate("Register", "\346\263\250\345\206\214", nullptr));
        Back->setText(QCoreApplication::translate("Register", "\350\277\224\345\233\236", nullptr));
        label->setText(QCoreApplication::translate("Register", "\346\230\265\347\247\260\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Register", "\345\257\206\347\240\201\357\274\232", nullptr));
        menu->setTitle(QCoreApplication::translate("Register", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Register: public Ui_Register {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERWINDOW_H
