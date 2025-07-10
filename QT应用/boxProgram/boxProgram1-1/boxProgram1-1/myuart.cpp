#include "myuart.h"

myuart::myuart(QSerialPort *serialPort): serial(serialPort) {}

void myuart::mySerialInit()
{
    //int outTime = 0;
    // 初始化节点设备ID
    mcuDev = -1;
    cameraDev = -1;
    // 配置串口参数
    serial->setPortName("/dev/ttyS1");  // 设置串口设备路径
    serial->setBaudRate(QSerialPort::Baud115200); // 设置波特率为115200
    serial->setDataBits(QSerialPort::Data8); // 设置数据位为8
    serial->setParity(QSerialPort::NoParity); // 无校验位
    serial->setStopBits(QSerialPort::OneStop); // 设置停止位为1
    serial->setFlowControl(QSerialPort::NoFlowControl); // 无流控制

    // 尝试打开串口
    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Failed to open port" << serial->portName() << "; Error:" << serial->errorString();
        return;
    }

    if(sendCommandAndWaitForResponse("AT\r\n","AT\r\n\r\nOK\r\n",2000)){
        qDebug() << "AT is OK!";
    } else {
        qDebug() << "ERROR:AT is no OK";
    }

    /*if(sendCommandAndWaitForResponse("AT+CWMODE_CUR=3\r\n","AT+CWMODE_CUR=3\r\n\r\nOK\r\n",2000)){
        qDebug() << "AT+CWMODE_CUR=3 is OK!";
    } else {
        qDebug() << "ERROR:AT+CWMODE_CUR=3 is no OK";
    }*/

    if(sendCommandAndWaitForResponse("AT+CIPMUX=1\r\n","AT+CIPMUX=1\r\n\r\nOK\r\n",3000)){
        qDebug() << "AT+CIPMUX=1 is OK!";
    } else {
        qDebug() << "ERROR:AT+CIPMUX=1 is no OK";
    }

    if(sendCommandAndWaitForResponse("AT+CWSAP_CUR=\"Redmi K40\",\"dgeUEHR12\",5,3\r\n","AT+CWSAP_CUR=\"Redmi K40\",\"dgeUEHR12\",5,3\r\n\r\nOK\r\n",5000)){
        qDebug() << "AT+CWSAP_CUR=\"Redmi K40\",\"dgeUEHR12\",5,3 is OK!";
    } else {
        qDebug() << "ERROR:AT+CWSAP_CUR=\"Redmi K40\",\"dgeUEHR12\",5,3 is no OK";
    }

    if(sendCommandAndWaitForResponse("AT+CIPSERVER=1,1001\r\n","AT+CIPSERVER=1,1001\r\n\r\nOK\r\n",5000)){
        qDebug() << "AT+CIPSERVER=1,1001 is OK!";
    } else {
        qDebug() << "ERROR:AT+CIPSERVER=1,1001 is no OK";
    }

    get_devID();
}

QString myuart::readMySerial()
{
    QString data = serial->readAll(); // 读取所有可用的数据
    qDebug() << "Received data:" << data;
    return data;
}

//向客户端发送数据的函数 参数1：要发送的字符串 参数2：目标客户端的ID号
void myuart::writeMySerial(const QByteArray &data,int linkID)
{
    QString commandString = "";
    if (serial->isWritable()) {

        readMySerial();//对缓存区进行清理
        serial->write("AT+CIPSTATUS\r\n");
        serial->flush();
        QString CIPSTATUS = clientReadData(3000);
        if (CIPSTATUS.contains("+CIPSTATUS:")) {
            QString partAfter = CIPSTATUS.section(':', 1); // 取冒号后的内容
            QString firstNumber = partAfter.split(',')[0].trimmed(); // 去掉后面内容
            linkID = firstNumber.toInt();
        }else{
            qDebug() << "ERROR: AT+CIPSTATUS is error";\

        }

        readMySerial(); //先对串口缓存区进行清理
        QString commandString = QString("AT+CIPSEND=%1,%2\r\n").arg(linkID).arg(data.size());
        QByteArray commandUtf = commandString.toUtf8();
        if(sendCommandAndWaitForResponse(commandUtf,">",4000)){
            qDebug() << "send data is success!";
        }else{
            qDebug() << "ERROR:send data passback outTime!";
        }
        serial->write(data);
        serial->flush();
    }
}

/*发送命令并等待回传的函数
 *参数：需要发送的命令 回传回来需要校验的命令 超时时间（毫秒）
 * 返回值 :True:校验到了回传数据 False:校验超时
*/
bool myuart::sendCommandAndWaitForResponse(const QByteArray &command,const QByteArray &expectedResponse,int timeoutMs)
{
    serial->write(command);
    serial->flush();
    receivedData.clear();

    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    // 关键修改：使用指针捕获局部变量
    auto onTimeout = [this]() {
        qDebug() << "Timeout! Received:" << receivedData;
    };

    // 关键修改：存储连接对象以便断开
    QMetaObject::Connection dataConnection;

    // 数据到达处理
    auto onDataReceived = [&, this]() {
        receivedData.append(serial->readAll());
        qDebug() << "Received:" << receivedData;  // 调试输出

        if (receivedData.endsWith(expectedResponse)) {
            // 关键：立即断开连接
            QObject::disconnect(dataConnection);
            loop.quit();
        }
    };

    // 连接信号
    dataConnection = connect(serial, &QSerialPort::readyRead, this, onDataReceived);
    connect(&timer, &QTimer::timeout, this, [&](){
        onTimeout();
        loop.quit();
    });

    timer.start(timeoutMs);
    loop.exec();

    // 确保退出时断开连接
    disconnect(dataConnection);
    timer.stop();

    return receivedData.endsWith(expectedResponse);
}

QByteArray myuart::clientReadData(int timeoutMs) {
    // 清空接收缓冲区
    receivedData.clear();

    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    // 存储解析结果
    QByteArray result;
    bool dataReceived = false;

    // 存储连接对象
    QMetaObject::Connection dataConnection;

    // 超时处理
    auto onTimeout = [&]() {
        qDebug() << "Timeout! Received:" << receivedData;
    };

    // 数据到达处理
    auto onDataReceived = [&, this]() {
        receivedData.append(serial->readAll());
        qDebug() << "Raw data received:" << receivedData;

        // 尝试解析数据包
        int ipdIndex = receivedData.indexOf("+IPD");
        if (ipdIndex == -1) return; // 没有包头

        // 查找冒号位置
        int colonIndex = receivedData.indexOf(':', ipdIndex);
        if (colonIndex == -1) return; // 没有找到冒号

        // 查找最后一个逗号（数据长度前）
        int lastCommaIndex = receivedData.lastIndexOf(',', colonIndex);
        if (lastCommaIndex == -1) return; // 格式错误

        // 提取数据长度
        QByteArray lenStr = receivedData.mid(lastCommaIndex + 1, colonIndex - lastCommaIndex - 1);
        bool ok;
        int dataLen = lenStr.toInt(&ok);

        if (!ok || dataLen <= 0) {
            qDebug() << "Invalid data length:" << lenStr;
            return;
        }

        // 检查是否收到完整数据
        if (receivedData.length() < colonIndex + 1 + dataLen) {
            qDebug() << "Partial data received. Expected:" << dataLen
                     << "Actual:" << (receivedData.length() - colonIndex - 1);
            return;
        }

        // 提取数据部分
        result = receivedData.mid(colonIndex + 1, dataLen);
        qDebug() << "Valid data received:" << result;

        // 标记已收到有效数据
        dataReceived = true;

        // 立即断开连接并退出事件循环
        if (dataConnection) QObject::disconnect(dataConnection);
        loop.quit();
    };

    // 连接信号
    dataConnection = connect(serial, &QSerialPort::readyRead, this, onDataReceived);
    connect(&timer, &QTimer::timeout, this, [&](){
        onTimeout();
        loop.quit();
    });

    timer.start(timeoutMs);
    loop.exec();

    // 清理
    if (dataConnection) disconnect(dataConnection);
    timer.stop();

    // 返回解析到的数据（超时返回空）
    return dataReceived ? result : QByteArray();
}

//记录设备的ID与实际设备相对应
void myuart::get_devID()
{
    writeMySerial("ID",0);
    QByteArray tempDevID = clientReadData(3000);
    if(QString(tempDevID) == "mcu"){
        mcuDev = 0;cameraDev = 1;
    }else if(QString(tempDevID) == "camera"){
        mcuDev = 1;cameraDev = 0;
    }
}
