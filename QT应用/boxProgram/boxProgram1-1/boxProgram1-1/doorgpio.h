#ifndef DOORGPIO_H
#define DOORGPIO_H

#include <QObject>
#include <QSocketNotifier>

class Doorgpio : public QObject {
    Q_OBJECT

public:
    explicit Doorgpio(QObject *parent = nullptr, int gpioNumber = 17);
    ~Doorgpio();

signals:
    void risingEdgeDetected();

private slots:
    void onGpioInterrupt();

private:
    int gpio_fd;
    int gpioNumber;
    QSocketNotifier *notifier;
    char lastValue = '0';  // 记录上次电平值

    bool setupGpio(int gpioNumber);
    bool writeSysfs(const char* path, const char* value);
};

#endif // DOORGPIO_H
