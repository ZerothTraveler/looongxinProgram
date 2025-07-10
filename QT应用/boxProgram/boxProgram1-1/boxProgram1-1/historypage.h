#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>

namespace Ui {
class historyPage;
}

class historyPage : public QWidget
{
    Q_OBJECT

public:
    explicit historyPage(QWidget *parent = nullptr);
    ~historyPage();

private:
    Ui::historyPage *ui;
};

#endif // HISTORYPAGE_H
