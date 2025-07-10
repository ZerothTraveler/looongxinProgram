/********************************************************************************
** Form generated from reading UI file 'historypage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HISTORYPAGE_H
#define UI_HISTORYPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_historyPage
{
public:
    QLabel *label;

    void setupUi(QWidget *historyPage)
    {
        if (historyPage->objectName().isEmpty())
            historyPage->setObjectName(QString::fromUtf8("historyPage"));
        historyPage->resize(770, 410);
        label = new QLabel(historyPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 67, 17));

        retranslateUi(historyPage);

        QMetaObject::connectSlotsByName(historyPage);
    } // setupUi

    void retranslateUi(QWidget *historyPage)
    {
        historyPage->setWindowTitle(QCoreApplication::translate("historyPage", "Form", nullptr));
        label->setText(QCoreApplication::translate("historyPage", "\345\216\206\345\217\262\346\237\245\350\257\242", nullptr));
    } // retranslateUi

};

namespace Ui {
    class historyPage: public Ui_historyPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HISTORYPAGE_H
