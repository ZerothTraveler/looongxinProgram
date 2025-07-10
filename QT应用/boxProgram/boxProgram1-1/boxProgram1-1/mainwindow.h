#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonDocument>
#include <QStandardItem>
#include <QJsonObject>
#include <QApplication>
#include <QJsonArray>
#include <QJsonValue>
#include <QTreeView>
#include <QProcess>
#include <QString>
#include "doorgpio.h"
#include "myuart.h"
#include <QDebug>
#include <QFile>
#include "log.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //TCP连接
    myuart *uartInstance;
    QSerialPort *tcpSerial;
    QByteArray mySerialBuffer;
    //QTcpSocket *socket;
    /*JSON数据库
      数据库对象*/
    QJsonObject jsonDatabase;
    QJsonObject ps1_1;
    QJsonObject ps1_2;
    QJsonObject ps2_1;
    QJsonObject ps2_2;
    QJsonObject ps3_1;
    QJsonObject ps3_2;
    QString dataTime;

    QJsonObject usingJsonDatabase;
    /*JSON数据库
      数据库函数*/
    void creatDatabase();

    void refreshDatabase();

    QString transformIndexes(int Indexes);

    double getPsWeight(QString position,QString type);

    QString getPsName(QString position);

    void setPsName(QString position,QString string);

    void setPsWeight(const QString &position, const QString &type, double value);

    void UsingJsonDatabase_Init();

    void saveJsonDatabase1(QString position,double allWeight);

    void overWriteJsonDatabase(const QByteArray &data);
    /*主页模块图片显示
      函数*/
    void psImageState(int position, bool state);

    void phImageStateInit();

    void refreshPhLable();

    void leasePageInit();

    void historyPageInit();

private slots:

    void on_homepageButton_toggled(bool checked);

    void on_historyButton_toggled(bool checked);

    void on_leaseButton_toggled(bool checked);

    void on_searchButton_clicked();

    //void myUartHardleDataReceived();
    void on_changeButton_clicked();

    void on_openButton_clicked();

    void lockRisingEdge();

private:
    Ui::MainWindow *ui;
    Doorgpio *doorGpio;
};
#endif // MAINWINDOW_H
