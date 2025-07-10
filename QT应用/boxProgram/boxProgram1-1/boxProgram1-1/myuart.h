#ifndef MYUART_H
#define MYUART_H

#include <QCoreApplication>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QObject>
#include <QDebug>
class myuart:public QMainWindow
{
public:

    explicit myuart(QSerialPort *serialPort);

    void mySerialInit();

    QString readMySerial();

    void writeMySerial(const QByteArray &data,int linkID);

    bool sendCommandAndWaitForResponse(const QByteArray &command, const QByteArray &expectedResponse, int timeoutMs);

    QByteArray clientReadData(int timeoutMs);

    void get_devID();

    //记录串口设备ID
    int mcuDev;

    int cameraDev;
private:

    QSerialPort *serial;

    QByteArray receivedData;
};

#endif // MYUART_H
