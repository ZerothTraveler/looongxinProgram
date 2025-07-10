#ifndef LOG_H
#define LOG_H

#include "mainwindow.h"
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QDebug>
#include <QFile>

namespace Ui {
class log;
}

class log : public QWidget
{
    Q_OBJECT

public:
    explicit log(QWidget *parent = nullptr);
    ~log();

    /*登录页面使用的串口通讯*/
    myuart *loguartInstance;
    QSerialPort *logSerial;
    /*用户信息的JSON数据库*/
    QJsonObject userJsonDatabase;

    static QString useringName;
    void userJsonDatabaseCreat();
    void creatUser(QString userName,QString userPassWord);
private slots:
    void on_longinButton_clicked();

    void on_enrollButton_clicked();

    void on_face_confirmButton_clicked();

    void on_acceptEnrollButton_clicked();

    void on_faceLonginButton_clicked();

private:
    Ui::log *ui;
};

#endif // LOG_H
