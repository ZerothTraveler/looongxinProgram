#include "mainwindow.h"
#include "log.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.resize(1024, 600);
    log *logWindow = new log;

    /************主页样式表初始化开始************/
    QFile homePageFile(":/sysle.qss");
    if(homePageFile.exists())
    {
        homePageFile.open(QFile::ReadOnly);
        QString homePageSheet = QLatin1String(homePageFile.readAll());
        qApp->setStyleSheet(homePageSheet);
        homePageFile.close();
    }
    /************主页样式表初始化结束************/
    //w.show();
    logWindow->show();
    return a.exec();
}

