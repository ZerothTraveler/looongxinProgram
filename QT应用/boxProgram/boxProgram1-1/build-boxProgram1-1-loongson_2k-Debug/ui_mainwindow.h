/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *line;
    QFrame *line_2;
    QPushButton *openButton;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *searchLineEdit;
    QPushButton *searchButton;
    QPushButton *changeButton;
    QFrame *frame;
    QStackedWidget *homeWidget;
    QWidget *homePage;
    QLabel *label_2;
    QPushButton *ps1_2Image;
    QLabel *ps1_2label;
    QPushButton *ps2_2Image;
    QPushButton *ps3_2Image;
    QLabel *ps2_2label;
    QLabel *ps3_2label;
    QPushButton *ps3_1Image;
    QLabel *ps3_1label;
    QLabel *ps2_1label;
    QPushButton *ps2_1Image;
    QLabel *ps1_1label;
    QPushButton *ps1_1Image;
    QWidget *rentPage;
    QTreeWidget *leaseTreeWidget;
    QWidget *historyPage;
    QTreeWidget *historyTreeWidget;
    QLabel *debugLabel;
    QLabel *timeLabel;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_4;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QRadioButton *homepageButton;
    QSpacerItem *verticalSpacer;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *leaseButton;
    QSpacerItem *verticalSpacer_2;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *historyButton;
    QLabel *usingLabel;
    QLabel *pageImage;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1024, 600);
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: white;\n"
"}\n"
"\n"
"QPushButton { \n"
"	background-color: rgba(174, 241, 255,200);\n"
"}\n"
"\n"
"QTreeWidget{\n"
"	outline:0px;\n"
"	background: #e8ffe8;\n"
"	color:#07689f;\n"
"	selection-background-color: transparent; \n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        line = new QFrame(centralwidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(830, 0, 20, 580));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralwidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(0, 60, 840, 15));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        openButton = new QPushButton(centralwidget);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setGeometry(QRect(870, 10, 130, 55));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(310, 10, 501, 41));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalSpacer_2 = new QSpacerItem(38, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        searchLineEdit = new QLineEdit(widget);
        searchLineEdit->setObjectName(QString::fromUtf8("searchLineEdit"));

        horizontalLayout->addWidget(searchLineEdit);

        searchButton = new QPushButton(widget);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));
        searchButton->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(searchButton);

        changeButton = new QPushButton(widget);
        changeButton->setObjectName(QString::fromUtf8("changeButton"));

        horizontalLayout->addWidget(changeButton);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 70, 821, 471));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        homeWidget = new QStackedWidget(frame);
        homeWidget->setObjectName(QString::fromUtf8("homeWidget"));
        homeWidget->setGeometry(QRect(10, 10, 801, 451));
        homePage = new QWidget();
        homePage->setObjectName(QString::fromUtf8("homePage"));
        label_2 = new QLabel(homePage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 10, 67, 17));
        ps1_2Image = new QPushButton(homePage);
        ps1_2Image->setObjectName(QString::fromUtf8("ps1_2Image"));
        ps1_2Image->setGeometry(QRect(430, 20, 90, 90));
        ps1_2label = new QLabel(homePage);
        ps1_2label->setObjectName(QString::fromUtf8("ps1_2label"));
        ps1_2label->setGeometry(QRect(440, 110, 90, 25));
        ps2_2Image = new QPushButton(homePage);
        ps2_2Image->setObjectName(QString::fromUtf8("ps2_2Image"));
        ps2_2Image->setGeometry(QRect(430, 140, 90, 90));
        ps3_2Image = new QPushButton(homePage);
        ps3_2Image->setObjectName(QString::fromUtf8("ps3_2Image"));
        ps3_2Image->setGeometry(QRect(430, 270, 90, 90));
        ps2_2label = new QLabel(homePage);
        ps2_2label->setObjectName(QString::fromUtf8("ps2_2label"));
        ps2_2label->setGeometry(QRect(440, 240, 90, 25));
        ps3_2label = new QLabel(homePage);
        ps3_2label->setObjectName(QString::fromUtf8("ps3_2label"));
        ps3_2label->setGeometry(QRect(440, 370, 90, 25));
        ps3_1Image = new QPushButton(homePage);
        ps3_1Image->setObjectName(QString::fromUtf8("ps3_1Image"));
        ps3_1Image->setGeometry(QRect(240, 270, 90, 90));
        ps3_1label = new QLabel(homePage);
        ps3_1label->setObjectName(QString::fromUtf8("ps3_1label"));
        ps3_1label->setGeometry(QRect(250, 370, 90, 25));
        ps2_1label = new QLabel(homePage);
        ps2_1label->setObjectName(QString::fromUtf8("ps2_1label"));
        ps2_1label->setGeometry(QRect(250, 240, 90, 25));
        ps2_1Image = new QPushButton(homePage);
        ps2_1Image->setObjectName(QString::fromUtf8("ps2_1Image"));
        ps2_1Image->setGeometry(QRect(240, 140, 90, 90));
        ps1_1label = new QLabel(homePage);
        ps1_1label->setObjectName(QString::fromUtf8("ps1_1label"));
        ps1_1label->setGeometry(QRect(250, 110, 90, 25));
        ps1_1Image = new QPushButton(homePage);
        ps1_1Image->setObjectName(QString::fromUtf8("ps1_1Image"));
        ps1_1Image->setGeometry(QRect(240, 20, 90, 90));
        homeWidget->addWidget(homePage);
        rentPage = new QWidget();
        rentPage->setObjectName(QString::fromUtf8("rentPage"));
        leaseTreeWidget = new QTreeWidget(rentPage);
        leaseTreeWidget->setObjectName(QString::fromUtf8("leaseTreeWidget"));
        leaseTreeWidget->setGeometry(QRect(0, 0, 801, 461));
        leaseTreeWidget->setColumnCount(3);
        homeWidget->addWidget(rentPage);
        historyPage = new QWidget();
        historyPage->setObjectName(QString::fromUtf8("historyPage"));
        historyTreeWidget = new QTreeWidget(historyPage);
        historyTreeWidget->setObjectName(QString::fromUtf8("historyTreeWidget"));
        historyTreeWidget->setGeometry(QRect(0, 0, 801, 461));
        homeWidget->addWidget(historyPage);
        debugLabel = new QLabel(centralwidget);
        debugLabel->setObjectName(QString::fromUtf8("debugLabel"));
        debugLabel->setGeometry(QRect(880, 90, 71, 17));
        timeLabel = new QLabel(centralwidget);
        timeLabel->setObjectName(QString::fromUtf8("timeLabel"));
        timeLabel->setGeometry(QRect(850, 460, 111, 17));
        widget_5 = new QWidget(centralwidget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setGeometry(QRect(860, 110, 147, 201));
        verticalLayout_4 = new QVBoxLayout(widget_5);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        widget_2 = new QWidget(widget_5);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: rgb(174, 241, 255);\n"
"	border-radius: 20px;\n"
"}"));
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        homepageButton = new QRadioButton(widget_2);
        homepageButton->setObjectName(QString::fromUtf8("homepageButton"));

        verticalLayout->addWidget(homepageButton);


        verticalLayout_4->addWidget(widget_2);

        verticalSpacer = new QSpacerItem(20, 18, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        widget_3 = new QWidget(widget_5);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: rgb(174, 241, 255);\n"
"	border-radius: 20px;\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(widget_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        leaseButton = new QRadioButton(widget_3);
        leaseButton->setObjectName(QString::fromUtf8("leaseButton"));

        verticalLayout_2->addWidget(leaseButton);


        verticalLayout_4->addWidget(widget_3);

        verticalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer_2);

        widget_4 = new QWidget(widget_5);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-color: rgb(174, 241, 255);\n"
"	border-radius: 20px;\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(widget_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        historyButton = new QRadioButton(widget_4);
        historyButton->setObjectName(QString::fromUtf8("historyButton"));

        verticalLayout_3->addWidget(historyButton);


        verticalLayout_4->addWidget(widget_4);

        usingLabel = new QLabel(centralwidget);
        usingLabel->setObjectName(QString::fromUtf8("usingLabel"));
        usingLabel->setGeometry(QRect(850, 440, 121, 23));
        pageImage = new QLabel(centralwidget);
        pageImage->setObjectName(QString::fromUtf8("pageImage"));
        pageImage->setGeometry(QRect(10, 0, 211, 60));
        pageImage->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    background-image: url(:/image/gameImage.png);\n"
"    background-position: center;\n"
"    background-repeat: no-repeat;\n"
"    }"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1024, 28));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        homeWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        openButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\351\227\250", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\347\224\265\345\255\220\346\250\241\345\235\227\346\231\272\350\203\275\347\247\237\345\200\237\347\263\273\347\273\237", nullptr));
        searchButton->setText(QCoreApplication::translate("MainWindow", "\346\220\234\347\264\242", nullptr));
        changeButton->setText(QCoreApplication::translate("MainWindow", "\346\233\264\346\224\271", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\344\270\273\351\241\265", nullptr));
        ps1_2Image->setText(QString());
        ps1_2label->setText(QCoreApplication::translate("MainWindow", "null", nullptr));
        ps2_2Image->setText(QString());
        ps3_2Image->setText(QString());
        ps2_2label->setText(QCoreApplication::translate("MainWindow", "null", nullptr));
        ps3_2label->setText(QCoreApplication::translate("MainWindow", "null", nullptr));
        ps3_1Image->setText(QString());
        ps3_1label->setText(QCoreApplication::translate("MainWindow", "null", nullptr));
        ps2_1label->setText(QCoreApplication::translate("MainWindow", "null", nullptr));
        ps2_1Image->setText(QString());
        ps1_1label->setText(QCoreApplication::translate("MainWindow", "null", nullptr));
        ps1_1Image->setText(QString());
        QTreeWidgetItem *___qtreewidgetitem = leaseTreeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264", nullptr));
        ___qtreewidgetitem->setText(1, QCoreApplication::translate("MainWindow", "\346\250\241\345\235\227\347\232\204\346\225\260\351\207\217", nullptr));
        ___qtreewidgetitem->setText(0, QCoreApplication::translate("MainWindow", "\346\250\241\345\235\227\345\220\215\347\247\260", nullptr));
        QTreeWidgetItem *___qtreewidgetitem1 = historyTreeWidget->headerItem();
        ___qtreewidgetitem1->setText(3, QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264", nullptr));
        ___qtreewidgetitem1->setText(2, QCoreApplication::translate("MainWindow", "\346\250\241\345\235\227\346\225\260\351\207\217", nullptr));
        ___qtreewidgetitem1->setText(1, QCoreApplication::translate("MainWindow", "\346\250\241\345\235\227\345\220\215\347\247\260", nullptr));
        ___qtreewidgetitem1->setText(0, QCoreApplication::translate("MainWindow", "\347\247\237\345\200\237\347\232\204\347\224\250\346\210\267", nullptr));
        debugLabel->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        timeLabel->setText(QCoreApplication::translate("MainWindow", "time", nullptr));
        homepageButton->setText(QCoreApplication::translate("MainWindow", "\344\270\273\351\241\265", nullptr));
        leaseButton->setText(QCoreApplication::translate("MainWindow", "\344\270\252\344\272\272\347\247\237\345\200\237\346\237\245\350\257\242", nullptr));
        historyButton->setText(QCoreApplication::translate("MainWindow", "\350\277\221\346\234\237\345\216\206\345\217\262\346\237\245\350\257\242", nullptr));
        usingLabel->setText(QCoreApplication::translate("MainWindow", "usering", nullptr));
        pageImage->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
