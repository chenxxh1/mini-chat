/********************************************************************************
** Form generated from reading UI file 'rejisterwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REJISTERWINDOW_H
#define UI_REJISTERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_rejisterwindow
{
public:
    QLineEdit *NameEdit;
    QLabel *Name;
    QLabel *label_2;
    QLineEdit *PassWord;
    QLabel *label;
    QLineEdit *ConfirmPassWord;
    QPushButton *Register;
    QPushButton *Back;

    void setupUi(QWidget *rejisterwindow)
    {
        if (rejisterwindow->objectName().isEmpty())
            rejisterwindow->setObjectName("rejisterwindow");
        rejisterwindow->resize(400, 300);
        NameEdit = new QLineEdit(rejisterwindow);
        NameEdit->setObjectName("NameEdit");
        NameEdit->setGeometry(QRect(90, 50, 151, 20));
        Name = new QLabel(rejisterwindow);
        Name->setObjectName("Name");
        Name->setGeometry(QRect(40, 50, 122, 22));
        label_2 = new QLabel(rejisterwindow);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(40, 100, 40, 20));
        PassWord = new QLineEdit(rejisterwindow);
        PassWord->setObjectName("PassWord");
        PassWord->setGeometry(QRect(90, 100, 151, 21));
        label = new QLabel(rejisterwindow);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 150, 61, 21));
        ConfirmPassWord = new QLineEdit(rejisterwindow);
        ConfirmPassWord->setObjectName("ConfirmPassWord");
        ConfirmPassWord->setGeometry(QRect(90, 150, 151, 21));
        Register = new QPushButton(rejisterwindow);
        Register->setObjectName("Register");
        Register->setGeometry(QRect(70, 210, 80, 18));
        Back = new QPushButton(rejisterwindow);
        Back->setObjectName("Back");
        Back->setGeometry(QRect(180, 210, 80, 18));

        retranslateUi(rejisterwindow);

        QMetaObject::connectSlotsByName(rejisterwindow);
    } // setupUi

    void retranslateUi(QWidget *rejisterwindow)
    {
        rejisterwindow->setWindowTitle(QCoreApplication::translate("rejisterwindow", "Form", nullptr));
        Name->setText(QCoreApplication::translate("rejisterwindow", "\346\230\265\347\247\260\357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("rejisterwindow", "\345\257\206\347\240\201\357\274\232", nullptr));
        label->setText(QCoreApplication::translate("rejisterwindow", "\351\207\215\346\226\260\347\241\256\350\256\244\345\257\206\347\240\201\357\274\232", nullptr));
        Register->setText(QCoreApplication::translate("rejisterwindow", "\346\263\250\345\206\214", nullptr));
        Back->setText(QCoreApplication::translate("rejisterwindow", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class rejisterwindow: public Ui_rejisterwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REJISTERWINDOW_H
