#include "historypage.h"
#include "ui_historypage.h"

historyPage::historyPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::historyPage)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
}

historyPage::~historyPage()
{
    delete ui;
}
