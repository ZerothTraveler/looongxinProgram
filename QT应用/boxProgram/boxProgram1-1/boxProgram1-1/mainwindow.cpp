#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "doorgpio.h"
#include "myuart.h"
#include "log.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), doorGpio(new Doorgpio(this))
{
    ui->setupUi(this);

    //tcp连接
    tcpSerial = new QSerialPort(this);
    uartInstance = new myuart(tcpSerial);
    //connect(&tcpSerial, &QSerialPort::readyRead, this, &MainWindow::myUartHardleDataReceived);
    //uartInstance->mySerialInit();

    //socket->connectToHost(QHostAddress("127.0.0.1"),8080);
    //设置页面多选框的初始值
    ui->homepageButton->setChecked(true);
    ui->leaseButton->setChecked(false);
    ui->historyButton->setChecked(false);

    //获取用户登录的时间存入dataTime中
    QDateTime tempDateTime= QDateTime::currentDateTime();
    dataTime = tempDateTime.toString("yyyy-MM-dd hh:mm:ss");
    ui->timeLabel->setText(dataTime);
    //creatDatabase();
    UsingJsonDatabase_Init();
    leasePageInit();
    ui->usingLabel->setText("当前用户:"+log::useringName);

    //初始化主页
    phImageStateInit();
    //初始化历史页面
    historyPageInit();

    //连接门锁处理信号
    connect(doorGpio, &Doorgpio::risingEdgeDetected, this, &MainWindow::lockRisingEdge);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete uartInstance;
    delete tcpSerial;
}

//门锁处理信号实现

void MainWindow::lockRisingEdge()
{
    qDebug() << "执行关门程序！";
    //执行重启
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
    QApplication::quit();

}

/*主页图片显示控制函数
 * 参数：图片位置 图片状态*/
void MainWindow::psImageState(int position, bool state)
{
    switch(position){
        case 1:
         if(state)
          {
              ui->ps1_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack0.png); background-color: transparent; }");

          }else{
              ui->ps1_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");
          }
        break;
        case 2:
         if(state)
          {
              ui->ps1_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack0.png); background-color: transparent; }");
          }else{
              ui->ps1_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");
          }
        break;
    default:
        qDebug() << "主页图片控制超出索引\n";
        break;
    }
}

//主页图片显示初始化函数
void MainWindow::phImageStateInit()
{
    ui->ps1_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack1.png); background-color: transparent; }");
    ui->ps1_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack1.png); background-color: transparent; }");
    ui->ps2_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack1.png); background-color: transparent; }");
    ui->ps2_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack1.png); background-color: transparent; }");
    ui->ps3_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack1.png); background-color: transparent; }");
    ui->ps3_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailBlack1.png); background-color: transparent; }");

    // 安全除法函数
    auto safeDivide = [](double numerator, double denominator) -> double {
        if (qFuzzyIsNull(denominator)) {  // 使用Qt的浮点数零检查
            qDebug() << "numerator ：\n" << numerator;
            qDebug() << "denominator ：\n" << denominator;
            qDebug() << "除法安全函数触发\n";
            return 0.0;  // 或者返回一个错误值，如 -1
        }
        return numerator / denominator;
    };

    QString tempName;
    double allWeight, oneWeight, tempAmount;

    // ps1_1
    tempName = getPsName("ps1_1");
    allWeight = getPsWeight("ps1_1","allWeight");
    oneWeight = getPsWeight("ps1_1","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps1_1label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps1_2
    tempName = getPsName("ps1_2");
    allWeight = getPsWeight("ps1_2","allWeight");
    oneWeight = getPsWeight("ps1_2","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps1_2label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps2_1
    tempName = getPsName("ps2_1");
    allWeight = getPsWeight("ps2_1","allWeight");
    oneWeight = getPsWeight("ps2_1","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps2_1label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps2_2
    tempName = getPsName("ps2_2");
    allWeight = getPsWeight("ps2_2","allWeight");
    oneWeight = getPsWeight("ps2_2","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps2_2label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps3_1
    tempName = getPsName("ps3_1");
    allWeight = getPsWeight("ps3_1","allWeight");
    oneWeight = getPsWeight("ps3_1","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps3_1label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps3_2
    tempName = getPsName("ps3_2");
    allWeight = getPsWeight("ps3_2","allWeight");
    oneWeight = getPsWeight("ps3_2","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps3_2label->setText(tempName + ":" + QString::number(tempAmount) + "个");
}
/************************页面选择槽函数***********************************/


void MainWindow::on_homepageButton_toggled(bool checked)
{
    if(checked == true){
        ui->leaseButton->setChecked(false);
        ui->historyButton->setChecked(false);
        ui->debugLabel->setText("主页被选中");
        ui->homeWidget->setCurrentIndex(0);
    }else{
    }
}

//更新主页标签的函数
void MainWindow::refreshPhLable()
{
    // 安全除法函数
    auto safeDivide = [](double numerator, double denominator) -> double {
        if (qFuzzyIsNull(denominator)) {  // 使用Qt的浮点数零检查
            qDebug() << "numerator ：\n" << numerator;
            qDebug() << "denominator ：\n" << denominator;
            qDebug() << "除法安全函数触发\n";
            return 0.0;  // 或者返回一个错误值，如 -1
        }
        return numerator / denominator;
    };

    QString tempName;
    double allWeight, oneWeight, tempAmount;

    // ps1_1
    tempName = getPsName("ps1_1");
    allWeight = getPsWeight("ps1_1","allWeight");
    oneWeight = getPsWeight("ps1_1","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps1_1label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps1_2
    tempName = getPsName("ps1_2");
    allWeight = getPsWeight("ps1_2","allWeight");
    oneWeight = getPsWeight("ps1_2","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps1_2label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps2_1
    tempName = getPsName("ps2_1");
    allWeight = getPsWeight("ps2_1","allWeight");
    oneWeight = getPsWeight("ps2_1","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps2_1label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps2_2
    tempName = getPsName("ps2_2");
    allWeight = getPsWeight("ps2_2","allWeight");
    oneWeight = getPsWeight("ps2_2","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps2_2label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps3_1
    tempName = getPsName("ps3_1");
    allWeight = getPsWeight("ps3_1","allWeight");
    oneWeight = getPsWeight("ps3_1","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps3_1label->setText(tempName + ":" + QString::number(tempAmount) + "个");

    // ps3_2
    tempName = getPsName("ps3_2");
    allWeight = getPsWeight("ps3_2","allWeight");
    oneWeight = getPsWeight("ps3_2","oneWeight");
    tempAmount = safeDivide(allWeight, oneWeight);
    ui->ps3_2label->setText(tempName + ":" + QString::number(tempAmount) + "个");
}

void MainWindow::on_leaseButton_toggled(bool checked)
{
    if(checked == true){
        ui->homepageButton->setChecked(false);
        ui->historyButton->setChecked(false);
        ui->debugLabel->setText("个人查询被选中");
        ui->homeWidget->setCurrentIndex(1);
        //overWriteJsonDatabase(uartInstance->clientReadData(3000));
    }else{}
}

//个人查询界面初始化函数
void MainWindow::leasePageInit()
{
    QString tempPosistion = "";
    QString tempName = "";
    QJsonValue tempValue;
    QJsonObject tempObject;//位置的对象
    QJsonObject tempUserObject;
    QJsonArray userArray;
    QString leaseTime;
    QJsonValue tempNameValue;
    double tempDoubleAllWeightValue = 0;
    double tempDoubleOneWeightValue = 0;
    for(int i = 0;i < 6;i++){
        tempPosistion = transformIndexes(i);
        tempValue = usingJsonDatabase.value(tempPosistion);
        tempObject = tempValue.toObject();
         if(tempObject.contains(log::useringName)){//如果有用户存储的记录则显示出来
            tempValue = tempObject.value("oneWeight");
            tempDoubleOneWeightValue = tempValue.toDouble();
            tempNameValue = tempObject.value("name");
            tempName = tempNameValue.toString();
            tempValue= tempObject.value(log::useringName);
            userArray = tempValue.toArray();
            for(int i = 0; i < userArray.count(); i++){
                tempValue = userArray.at(i);
                tempUserObject = tempValue.toObject();
                tempValue = tempUserObject.value("time");
                leaseTime = tempValue.toString();
                tempValue = tempUserObject.value("weight");
                tempDoubleAllWeightValue = tempValue.toDouble();
                QTreeWidgetItem *leaseitem = new QTreeWidgetItem();
                leaseitem->setText(0, tempName);
                leaseitem->setText(1, QString::number(tempDoubleAllWeightValue/tempDoubleOneWeightValue));
                leaseitem->setText(2, leaseTime);
                ui->leaseTreeWidget->addTopLevelItem(leaseitem);
            }
        }
    }
}

void MainWindow::on_historyButton_toggled(bool checked)
{
    if(checked == true){
        ui->homepageButton->setChecked(false);
        ui->leaseButton->setChecked(false);
        ui->homeWidget->setCurrentIndex(2);
    }else{
    }
}

//历史界面初始化函数
void MainWindow::historyPageInit()
{

    QString tempPosistion = "";
    QJsonValue tempValue;
    QJsonObject positionObject;
    double tempDoubleAllWeightValue = 0;
    double tempDoubleOneWeightValue = 0;
    QString tempName;
    QString leaseTime;
    QJsonObject tempUserObject;
    QJsonArray userArray;
    QFile userAcquireFile("//get//userJsonDatabase.json");
    userAcquireFile.open(QFile::ReadOnly);
    QByteArray jsonData = userAcquireFile.readAll();
    userAcquireFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonObject userAcquireObject = doc.object();
    QStringList keys = userAcquireObject.keys();
    foreach(const QString &key, keys){
        for(int i = 0;i < 6;i++){
            tempPosistion = transformIndexes(i);
            tempValue = usingJsonDatabase.value(tempPosistion);
            positionObject = tempValue.toObject();
            if(positionObject.contains(key)){
                tempValue = positionObject.value("oneWeight");
                tempDoubleOneWeightValue = tempValue.toDouble();
                tempValue = positionObject.value("name");
                tempName = tempValue.toString();
                tempValue= positionObject.value(key);
                userArray = tempValue.toArray();
                for(int i = 0; i < userArray.count(); i++){
                    tempValue = userArray.at(i);
                    tempUserObject = tempValue.toObject();
                    tempValue = tempUserObject.value("time");
                    leaseTime = tempValue.toString();
                    tempValue = tempUserObject.value("weight");
                    tempDoubleAllWeightValue = tempValue.toDouble();
                    QTreeWidgetItem *leaseitem = new QTreeWidgetItem();
                    leaseitem->setText(0, key);
                    leaseitem->setText(1, tempName);
                    leaseitem->setText(2, QString::number(tempDoubleAllWeightValue/tempDoubleOneWeightValue));
                    leaseitem->setText(3, leaseTime);
                    ui->historyTreeWidget->addTopLevelItem(leaseitem);
                }
            }
         }
     }
}
/************************JSON数据库函数***********************************/
void MainWindow::creatDatabase()
{
    //1-1位置
    ps1_1.insert("name","Nixie_tube");
    ps1_1.insert("oneWeight",2);
    ps1_1.insert("allWeight",10);

    QJsonObject rootObject;
    rootObject.insert("weight",2);
    rootObject.insert("time",dataTime);

    QJsonArray rootArray;
    rootArray.append(rootObject);
    ps1_1.insert("root",rootArray);
    jsonDatabase.insert("ps1_1",ps1_1);
    //1-2位置
    ps1_2.insert("name","HC05");
    ps1_2.insert("oneWeight",3);
    ps1_2.insert("allWeight",12);
    jsonDatabase.insert("ps1_2",ps1_2);

    //2-1位置
    ps2_1.insert("name","Current_transformer");
    ps2_1.insert("oneWeight",4);
    ps2_1.insert("allWeight",12);
    jsonDatabase.insert("ps2_1",ps2_1);

    //2-2位置
    ps2_2.insert("name","Flame_Sensor");
    ps2_2.insert("oneWeight",4);
    ps2_2.insert("allWeight",12);
    jsonDatabase.insert("ps2_2",ps2_2);

    //3-1位置
    ps3_1.insert("name","Stm32F103");
    ps3_1.insert("oneWeight",4.5);
    ps3_1.insert("allWeight",4.5);
    jsonDatabase.insert("ps3_1",ps3_1);

    //3-2位置
    ps3_2.insert("name","Relay");
    ps3_2.insert("oneWeight",4);
    ps3_2.insert("allWeight",12);
    jsonDatabase.insert("ps3_2",ps3_2);

    //创建JSON数据库文件并写入初始化
    QJsonDocument docData(jsonDatabase);
    QByteArray jsonData = docData.toJson();
    QFile dataFile("//get//jsonDatabase.json");
    dataFile.open(QFile::WriteOnly);
    dataFile.write(jsonData);
    dataFile.close();
}

//刷新数据库
void MainWindow::refreshDatabase()
{
    QJsonDocument docData(usingJsonDatabase);
    QByteArray jsonData = docData.toJson();
    QFile dataFile("//get//jsonDatabase.json");
    dataFile.open(QFile::WriteOnly);
    dataFile.write(jsonData);
    dataFile.close();
}

//将对应索引(0-6)转换成对应位置的字符串
QString MainWindow::transformIndexes(int Indexes)
{
    switch(Indexes){
        case 0:
        return "ps1_1";break;
        case 1:
        return "ps1_2";break;
        case 2:
        return "ps2_1";break;
        case 3:
        return "ps2_2";break;
        case 4:
        return "ps3_1";break;
        case 5:
        return "ps3_2";break;
    }
    qDebug() << "错误：超出转换的索引\n";
    return "error";
}


/*正在使用的JSON数据库初始化函数
 *将文件中的JSON对象读出转换至usingJsonDatabase对象
 */
void MainWindow::UsingJsonDatabase_Init()
{
    QFile dataFile("//get//jsonDatabase.json");
    dataFile.open(QFile::ReadOnly);
    QByteArray jsonData = dataFile.readAll();
    dataFile.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    usingJsonDatabase = doc.object();
}

/*获取存储库相应位置模块的单个重量或者总重量
 * 参数： 1.位置 "ps1_1"
 *       2."oneWeight" 或 "allWeight"
 */
double MainWindow::getPsWeight(QString position,QString type)
{
    QJsonValue tempValue = usingJsonDatabase.value(position);
    QJsonObject tempObject = tempValue.toObject();
    QJsonValue resultValue = tempObject.value(type);
    double result = resultValue.toDouble();
    return result;
}

/*获取存储库相应位置模块的名称
 * 参数： 1.位置 "ps1_1"
 */
QString MainWindow::getPsName(QString position)
{
    QJsonValue tempValue = usingJsonDatabase.value(position);
    QJsonObject tempObject = tempValue.toObject();
    QJsonValue resultValue = tempObject.value("name");
    QString result = resultValue.toString();
    return result;
}

/*修改对应位置的模块名称函数*/
void MainWindow::setPsName(QString position,QString name)
{
    QJsonValue tempValue = usingJsonDatabase.value(position);
    QJsonObject tempObject = tempValue.toObject();
    tempObject["name"] = name;
    usingJsonDatabase[position] = tempObject;
}

/*修改对应位置的单重或总重的函数*/
void MainWindow::setPsWeight(const QString &position, const QString &type, double value)
{
    // 先检查是否包含该 key
    if (usingJsonDatabase.contains(position)) {
        // 获取当前对象
        QJsonObject tempObject = usingJsonDatabase.value(position).toObject();

        // 修改指定字段
        tempObject[type] = value;
        usingJsonDatabase[position] = tempObject;
    } else {
         qDebug() << "setPsWeight ERROR:输入位置超出索引\n";
    }
}


//用于存储用户改变的重量的函数（其中有写入存储的用户的功能）
void MainWindow::saveJsonDatabase1(QString position,double allWeight)
{
    QJsonValue tempValue = usingJsonDatabase.value(position);
    QJsonObject tempObject = tempValue.toObject();
    QJsonValue tempAllWeightValue = tempObject.value("allWeight");
    double tempAllWeightValue1 = tempAllWeightValue.toDouble();
    if(allWeight != tempAllWeightValue1){
        double subtractionValue = allWeight - tempAllWeightValue1;
        if(tempObject.contains(log::useringName)){//检测这个用户是否存在历史存储
            double historyAllWeight = 0;
            QJsonValue userAcquireArrayValue=tempObject.value(log::useringName);
            QJsonArray userAcquireArray = userAcquireArrayValue.toArray();
            for(int i = 0;i < userAcquireArray.count();i++){
                QJsonValue userAcquireObjectValue = userAcquireArray.at(i);
                QJsonObject userAcquireObject = userAcquireObjectValue.toObject();
                QJsonValue acquireWeightValue = userAcquireObject.value("weight");
                historyAllWeight = historyAllWeight + acquireWeightValue.toDouble();
            }
            if(historyAllWeight == 0){//判断是否归还完
                tempObject.remove(log::useringName);
            }else{//没有归还
                QJsonObject storageObject;
                storageObject.insert("weight",subtractionValue);
                storageObject.insert("time",dataTime);
                userAcquireArray.append(storageObject);
                tempObject[log::useringName] = userAcquireArray;
            }
            usingJsonDatabase[position] = tempObject;
            setPsWeight(position,"allWeight",allWeight);
        }else{//如果没有储存则创建一个对象
            QJsonObject storageObject;
            storageObject.insert("weight",subtractionValue);
            storageObject.insert("time",dataTime);
            QJsonArray tempUserArray;
            tempUserArray.append(storageObject);
            tempObject[log::useringName] = tempUserArray;
            setPsWeight(position,"allWeight",allWeight);
        }
    }
}

//存储结束结算函数
void MainWindow::overWriteJsonDatabase(const QByteArray &data)
{
    QList<double> doubleegers;
    qDebug() << "send";
    // 将 QByteArray 转换为 QString
    QString str(data);

    // 使用空格分割字符串
    QStringList parts = str.split(' ', Qt::SkipEmptyParts);

    // 遍历每个部分并尝试将其转换为整数
    for (const QString &part : parts) {
        bool ok;
        double number = part.toDouble(&ok);
        if (ok) {
            doubleegers.append(number);
        } else {
            qDebug() << "Failed to convert" << part << "to an doubleeger.";
        }
    }

    double positionIndex = 0;
    for(positionIndex = 0;positionIndex < 6;positionIndex++){
        saveJsonDatabase1(transformIndexes(positionIndex),doubleegers.at(positionIndex));
    }
    refreshDatabase();
}

void MainWindow::on_searchButton_clicked()/****************************************************************/
{
    QString searchName = ui->searchLineEdit->text();
    QString tempPosition = " ";
    int i = 0;
    for(i = 0;i < 6;i++){
        tempPosition = transformIndexes(i);
        if(searchName == getPsName(tempPosition)){
            switch(i){
            case 0:ui->ps1_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");break;
            case 1:ui->ps1_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");break;
            case 2:ui->ps2_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");break;
            case 3:ui->ps2_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");break;
            case 4:ui->ps3_1Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");break;
            case 5:ui->ps3_2Image->setStyleSheet("QPushButton { background-image: url(:/image/detailGreen1.png); border: none; background-color: transparent; }");break;
            }
        break;
        }
    }
}

void MainWindow::on_changeButton_clicked()
{
    if (log::useringName == "root") {
        QString changeName = ui->searchLineEdit->text();
        QStringList changeStringList = changeName.split(' ', Qt::SkipEmptyParts);

        if (changeStringList.isEmpty()) {
            qDebug() << "Input is empty.";
            return;
        }

        QString changeKey = changeStringList.at(0);

        if (changeKey == "1-1") {
            uartInstance->writeMySerial("0",uartInstance->mcuDev);
            QByteArray changeOneWeightQByte = uartInstance->clientReadData(3000);
            double changeOneWeight = changeOneWeightQByte.toDouble();
            setPsWeight("ps1_1","oneWeight",changeOneWeight);
            setPsWeight("ps1_1","allWeight",0);
            setPsName("ps1_1", changeStringList.at(1));
            refreshDatabase();
            refreshPhLable();
        } else if (changeKey == "1-2") {
            uartInstance->writeMySerial("1",uartInstance->mcuDev);
            QByteArray changeOneWeightQByte = uartInstance->clientReadData(3000);
            double changeOneWeight = changeOneWeightQByte.toDouble();
            setPsWeight("ps1_2","oneWeight",changeOneWeight);
            setPsWeight("ps1_2","allWeight",0);
            setPsName("ps1_2", changeStringList.at(1));
            refreshDatabase();
            refreshPhLable();
        } else if (changeKey == "2-1") {
            uartInstance->writeMySerial("2",uartInstance->mcuDev);
            QByteArray changeOneWeightQByte = uartInstance->clientReadData(3000);
            double changeOneWeight = changeOneWeightQByte.toDouble();
            setPsWeight("ps2_1","oneWeight",changeOneWeight);
            setPsWeight("ps2_1","allWeight",0);
            setPsName("ps2_1", changeStringList.at(1));
            refreshDatabase();
            refreshPhLable();
        } else if (changeKey == "2-2") {
            uartInstance->writeMySerial("3",uartInstance->mcuDev);
            QByteArray changeOneWeightQByte = uartInstance->clientReadData(3000);
            double changeOneWeight = changeOneWeightQByte.toDouble();
            setPsWeight("ps2_2","oneWeight",changeOneWeight);
            setPsWeight("ps2_2","allWeight",0);
            setPsName("ps2_2", changeStringList.at(1));
            refreshDatabase();
            refreshPhLable();
        } else if (changeKey == "3-1") {
            uartInstance->writeMySerial("4",uartInstance->mcuDev);
            QByteArray changeOneWeightQByte = uartInstance->clientReadData(3000);
            double changeOneWeight = changeOneWeightQByte.toDouble();
            setPsWeight("ps3_1","oneWeight",changeOneWeight);
            setPsWeight("ps3_1","allWeight",0);
            setPsName("ps3_1", changeStringList.at(1));
            refreshDatabase();
            refreshPhLable();
        } else if (changeKey == "3-2") {
            uartInstance->writeMySerial("5",uartInstance->mcuDev);
            QByteArray changeOneWeightQByte = uartInstance->clientReadData(3000);
            double changeOneWeight = changeOneWeightQByte.toDouble();
            setPsWeight("ps3_2","oneWeight",changeOneWeight);
            setPsWeight("ps3_2","allWeight",0);
            setPsName("ps3_2", changeStringList.at(1));
            refreshDatabase();
            refreshPhLable();
        } else {
            qDebug() << "Unknown key:" << changeKey;
        }
    }
}

void MainWindow::on_openButton_clicked()
{
    uartInstance->writeMySerial("over",uartInstance->mcuDev);
    overWriteJsonDatabase(uartInstance->clientReadData(3000));
    refreshDatabase();
    refreshPhLable();
}
