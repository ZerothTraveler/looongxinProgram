#include "doorgpio.h"
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>
#include <cstdio>

Doorgpio::Doorgpio(QObject *parent, int gpioNumber)
    : QObject(parent), gpio_fd(-1), gpioNumber(gpioNumber), notifier(nullptr)
{
    if (setupGpio(gpioNumber)) {
        qDebug() << "GPIO" << gpioNumber << "设置成功，等待上升沿...";
    } else {
        qDebug() << "GPIO" << gpioNumber << "设置失败";
    }
}

Doorgpio::~Doorgpio()
{
    if (notifier) {
        notifier->setEnabled(false);
        delete notifier;
    }

    if (gpio_fd != -1) {
        close(gpio_fd);
    }

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "echo %d > /sys/class/gpio/unexport", gpioNumber);
    system(buffer);
}

bool Doorgpio::writeSysfs(const char* path, const char* value) {
    FILE* fp = fopen(path, "w");
    if (!fp) {
        qDebug() << "无法打开" << path << ":" << strerror(errno);
        return false;
    }

    if (fprintf(fp, "%s", value) < 0) {
        qDebug() << "写入失败" << path << ":" << strerror(errno);
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

bool Doorgpio::setupGpio(int gpioNumber)
{
    char path[50];

    // 导出GPIO
    snprintf(path, sizeof(path), "/sys/class/gpio/export");
    char value[10];
    snprintf(value, sizeof(value), "%d", gpioNumber);
    if (!writeSysfs(path, value)) {
        return false;
    }

    // 等待GPIO目录创建完成
    usleep(100000);  // 100ms

    // 设置方向为输入
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", gpioNumber);
    if (!writeSysfs(path, "in")) {
        return false;
    }

    // 设置中断触发方式为上升沿
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/edge", gpioNumber);
    if (!writeSysfs(path, "rising")) {
        return false;
    }

    // 打开值文件
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpioNumber);
    gpio_fd = open(path, O_RDWR | O_NONBLOCK);
    if (gpio_fd < 0) {
        qDebug() << "无法打开value文件:" << strerror(errno);
        return false;
    }

    // 读取初始值
    char c;
    if (read(gpio_fd, &c, 1) > 0) {
        lastValue = c;
        lseek(gpio_fd, 0, SEEK_SET);
    }

    // 创建socket通知器
    notifier = new QSocketNotifier(gpio_fd, QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &Doorgpio::onGpioInterrupt);

    return true;
}

void Doorgpio::onGpioInterrupt()
{
    char c;
    lseek(gpio_fd, 0, SEEK_SET);

    if (read(gpio_fd, &c, 1) > 0) {
        // 检测上升沿：当前为高电平且上次为低电平
        if (c == '1' && lastValue == '0') {
            qDebug() << "检测到上升沿! GPIO状态:" << c;
            emit risingEdgeDetected();
        }

        lastValue = c;  // 更新上次电平值
    }
}
