/********************************************************************************
** Form generated from reading UI file 'log.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOG_H
#define UI_LOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_log
{
public:
    QFrame *frame;
    QStackedWidget *logStackedWidget;
    QWidget *page;
    QLineEdit *userLineEdit;
    QLabel *label;
    QPushButton *longinButton;
    QPushButton *enrollButton;
    QLineEdit *passwordLineEdit;
    QLabel *label_2;
    QLabel *feedbackLabel;
    QPushButton *faceLonginButton;
    QWidget *page_2;
    QPushButton *face_transcribeButton;
    QPushButton *face_confirmButton;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *enrollUsernameLineEdit;
    QLabel *label_5;
    QLineEdit *enrollPasswareLineEdit;
    QPushButton *acceptEnrollButton;
    QLabel *debugRegisterLabel;

    void setupUi(QWidget *log)
    {
        if (log->objectName().isEmpty())
            log->setObjectName(QString::fromUtf8("log"));
        log->resize(250, 300);
        log->setStyleSheet(QString::fromUtf8("QFrame\n"
"{\n"
"	background-color: rgb(108,108,108);\n"
"	border-radius:12px;\n"
"}\n"
"QLabel\n"
"{\n"
"	color:white;\n"
"}\n"
"QLineEdit\n"
"{\n"
"	border-radius:6px;\n"
"}\n"
"QPushButton\n"
"{\n"
"	border-radius:6px;\n"
"	background-color:rgb(138, 226, 52);\n"
"}\n"
"QPushButton:pressed\n"
"{\n"
"	background-color:rgb(170,170,0);\n"
"}\n"
"QStackedWidget\n"
"{\n"
"	background-color:rgb(108,108,108);\n"
"}"));
        frame = new QFrame(log);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 251, 301));
        frame->setStyleSheet(QString::fromUtf8(""));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        logStackedWidget = new QStackedWidget(frame);
        logStackedWidget->setObjectName(QString::fromUtf8("logStackedWidget"));
        logStackedWidget->setGeometry(QRect(10, 10, 231, 281));
        logStackedWidget->setStyleSheet(QString::fromUtf8(""));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        userLineEdit = new QLineEdit(page);
        userLineEdit->setObjectName(QString::fromUtf8("userLineEdit"));
        userLineEdit->setGeometry(QRect(30, 50, 171, 31));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 67, 17));
        longinButton = new QPushButton(page);
        longinButton->setObjectName(QString::fromUtf8("longinButton"));
        longinButton->setGeometry(QRect(10, 190, 102, 31));
        longinButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius:6px;\n"
"	background-color:rgb(114, 159, 207);\n"
"}"));
        enrollButton = new QPushButton(page);
        enrollButton->setObjectName(QString::fromUtf8("enrollButton"));
        enrollButton->setGeometry(QRect(120, 190, 102, 31));
        enrollButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius:6px;\n"
"	background-color:rgb(114, 159, 207);\n"
"}"));
        passwordLineEdit = new QLineEdit(page);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setGeometry(QRect(30, 130, 171, 31));
        label_2 = new QLabel(page);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 110, 67, 17));
        feedbackLabel = new QLabel(page);
        feedbackLabel->setObjectName(QString::fromUtf8("feedbackLabel"));
        feedbackLabel->setGeometry(QRect(60, 170, 111, 20));
        faceLonginButton = new QPushButton(page);
        faceLonginButton->setObjectName(QString::fromUtf8("faceLonginButton"));
        faceLonginButton->setGeometry(QRect(60, 230, 102, 31));
        faceLonginButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius:6px;\n"
"	background-color:rgb(114, 159, 207);\n"
"}"));
        logStackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        face_transcribeButton = new QPushButton(page_2);
        face_transcribeButton->setObjectName(QString::fromUtf8("face_transcribeButton"));
        face_transcribeButton->setGeometry(QRect(10, 190, 102, 31));
        face_transcribeButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius:6px;\n"
"	background-color:rgb(114, 159, 207);\n"
"}"));
        face_confirmButton = new QPushButton(page_2);
        face_confirmButton->setObjectName(QString::fromUtf8("face_confirmButton"));
        face_confirmButton->setGeometry(QRect(120, 190, 102, 31));
        face_confirmButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius:6px;\n"
"	background-color:rgb(114, 159, 207);\n"
"}"));
        label_3 = new QLabel(page_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 170, 211, 21));
        label_4 = new QLabel(page_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 30, 91, 17));
        enrollUsernameLineEdit = new QLineEdit(page_2);
        enrollUsernameLineEdit->setObjectName(QString::fromUtf8("enrollUsernameLineEdit"));
        enrollUsernameLineEdit->setGeometry(QRect(30, 50, 171, 31));
        label_5 = new QLabel(page_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 110, 67, 17));
        enrollPasswareLineEdit = new QLineEdit(page_2);
        enrollPasswareLineEdit->setObjectName(QString::fromUtf8("enrollPasswareLineEdit"));
        enrollPasswareLineEdit->setGeometry(QRect(30, 130, 171, 31));
        acceptEnrollButton = new QPushButton(page_2);
        acceptEnrollButton->setObjectName(QString::fromUtf8("acceptEnrollButton"));
        acceptEnrollButton->setGeometry(QRect(60, 230, 102, 31));
        acceptEnrollButton->setStyleSheet(QString::fromUtf8("QPushButton\n"
"{\n"
"	border-radius:6px;\n"
"	background-color:rgb(114, 159, 207);\n"
"}"));
        debugRegisterLabel = new QLabel(page_2);
        debugRegisterLabel->setObjectName(QString::fromUtf8("debugRegisterLabel"));
        debugRegisterLabel->setGeometry(QRect(60, 10, 111, 20));
        logStackedWidget->addWidget(page_2);

        retranslateUi(log);

        logStackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(log);
    } // setupUi

    void retranslateUi(QWidget *log)
    {
        log->setWindowTitle(QCoreApplication::translate("log", "Form", nullptr));
        label->setText(QCoreApplication::translate("log", "\347\224\250\346\210\267\345\220\215", nullptr));
        longinButton->setText(QCoreApplication::translate("log", "\347\231\273\345\275\225", nullptr));
        enrollButton->setText(QCoreApplication::translate("log", "\346\263\250\345\206\214", nullptr));
        label_2->setText(QCoreApplication::translate("log", "\345\257\206\347\240\201", nullptr));
        feedbackLabel->setText(QString());
        faceLonginButton->setText(QCoreApplication::translate("log", "\344\272\272\350\204\270\347\231\273\345\275\225", nullptr));
        face_transcribeButton->setText(QCoreApplication::translate("log", "\344\272\272\350\204\270\345\275\225\345\210\266", nullptr));
        face_confirmButton->setText(QCoreApplication::translate("log", "\347\241\256\350\256\244\347\273\221\345\256\232", nullptr));
        label_3->setText(QCoreApplication::translate("log", "\346\217\220\347\244\272\357\274\232\347\202\271\345\207\273\345\233\233\346\254\241\345\275\225\345\210\266\350\257\206\345\210\253\346\233\264\347\262\276\345\207\206", nullptr));
        label_4->setText(QCoreApplication::translate("log", "\346\263\250\345\206\214\347\224\250\346\210\267\345\220\215", nullptr));
        label_5->setText(QCoreApplication::translate("log", "\346\263\250\345\206\214\345\257\206\347\240\201", nullptr));
        acceptEnrollButton->setText(QCoreApplication::translate("log", "\347\241\256\350\256\244\346\263\250\345\206\214", nullptr));
        debugRegisterLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class log: public Ui_log {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOG_H
