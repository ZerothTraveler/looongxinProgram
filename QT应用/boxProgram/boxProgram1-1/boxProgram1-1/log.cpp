#include "log.h"
#include "ui_log.h"

QString log::useringName = "";

log::log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::log)
{
    ui->setupUi(this);
    userJsonDatabaseCreat();
    logSerial = new QSerialPort(this);
    loguartInstance = new myuart(logSerial);
    loguartInstance->mySerialInit();
}

log::~log()
{
    delete ui;
}

void log::on_longinButton_clicked()
{
    QString userName = ui->userLineEdit->text();
    QString userPassword = ui->passwordLineEdit->text();
    QFile dataFile("//get//userJsonDatabase.json");
    dataFile.open(QFile::ReadOnly);
    QByteArray jsonData = dataFile.readAll();
    dataFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    userJsonDatabase = doc.object();

    foreach (const QString &key, userJsonDatabase.keys()) {
        if(key == userName){
            QJsonValue value = userJsonDatabase.value(key);
            if(value == userPassword){
                ui->feedbackLabel->setText("登录成功");
                qDebug() << "登录成功\n";
                useringName = userName;
                this->hide();
                MainWindow *homePage = new MainWindow;
                homePage->show();
                return;
            }
        }
    }
    ui->feedbackLabel->setText("用户名或密码错误");
    qDebug() << "用户名或密码错误\n";
    return;
}

void log::on_enrollButton_clicked()
{
    ui->logStackedWidget->setCurrentIndex(1);
}

void log::userJsonDatabaseCreat()
{
    userJsonDatabase.insert("root","123456");
    userJsonDatabase.insert("useringName","root");
    QJsonDocument docData(userJsonDatabase);
    QByteArray jsonData = docData.toJson();
    QFile dataFile("//get//userJsonDatabase.json");
    dataFile.open(QFile::WriteOnly);
    dataFile.write(jsonData);
    dataFile.close();
}

void log::creatUser(QString userName,QString userPassWord)
{
    QFile dataFile("//get//userJsonDatabase.json");
    dataFile.open(QFile::ReadOnly);
    QByteArray jsonData = dataFile.readAll();
    dataFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    userJsonDatabase = doc.object();

    userJsonDatabase.insert(userName,userPassWord);

    QJsonDocument docData(userJsonDatabase);
    jsonData = docData.toJson();
    dataFile.open(QFile::WriteOnly);
    dataFile.write(jsonData);
    dataFile.close();
}

void log::on_face_confirmButton_clicked()
{
    ui->logStackedWidget->setCurrentIndex(0);
}

void log::on_acceptEnrollButton_clicked()
{
    QString userName = ui->enrollUsernameLineEdit->text();
    QString userPassWord = ui->enrollPasswareLineEdit->text();
    QFile dataFile("//get//userJsonDatabase.json");
    dataFile.open(QFile::ReadOnly);
    QByteArray jsonData = dataFile.readAll();
    dataFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    userJsonDatabase = doc.object();

    userJsonDatabase.insert(userName,userPassWord);

    QJsonDocument docData(userJsonDatabase);
    jsonData = docData.toJson();
    dataFile.open(QFile::WriteOnly);
    dataFile.write(jsonData);
    dataFile.close();
    ui->debugRegisterLabel->setText("注册成功");
}

void log::on_faceLonginButton_clicked()
{
    QString camerUsernameString = ""; //让相机进行登录识别的命令
    loguartInstance->writeMySerial("",loguartInstance->cameraDev);
    QByteArray tempCamerByteArray = loguartInstance->clientReadData(3000);
    camerUsernameString = QString(tempCamerByteArray);
    qDebug() << "摄像头传来的用户名:" << camerUsernameString << Qt::endl;

    QFile dataFile("//get//userJsonDatabase.json");
    dataFile.open(QFile::ReadOnly);
    QByteArray jsonData = dataFile.readAll();
    dataFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    userJsonDatabase = doc.object();

    foreach (const QString &key, userJsonDatabase.keys()) {
        if(key == camerUsernameString){
            ui->feedbackLabel->setText("登录成功");
            qDebug() << "登录成功\n";
            useringName = camerUsernameString;
            this->hide();
            MainWindow *homePage = new MainWindow;
            homePage->show();
            return;
        }
    }
    ui->feedbackLabel->setText("用户名或密码错误");
    qDebug() << "用户名或密码错误\n";
    return;
}
