#ifndef HOMEPAGE_H
#define HOMEPAGE_H

//#include <QWidget>
#include "qmainwindow.h"
namespace Ui {
class homePage;
}

class homePage : public QWidget
{
    Q_OBJECT

public:
    explicit homePage(QWidget *parent = nullptr);
    ~homePage();

private:
    Ui::homePage *ui;
};

#endif // HOMEPAGE_H
