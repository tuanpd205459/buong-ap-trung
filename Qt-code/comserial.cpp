#include "comserial.h"
#include <QMainWindow>

// Constructor
comserial::comserial(QSerialPort *myDev)
{
    devSerial = myDev;
}

// Phương thức kết nối với các thiết bị nối tiếp và trả về danh sách các cổng nối tiếp có sẵn
QStringList comserial::ConnectDevices()
{
    QStringList devs;

    foreach (const QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
        devSerial->setPort(info);

        if (devSerial->open(QIODevice::ReadWrite)) {
            devSerial->close();
            devs << info.portName();
        }
    }
    return devs;
}

// Phương thức kết nối với cổng nối tiếp được chọn
bool comserial::ConnectCom(QString portname)
{
    devSerial->setPortName(portname);
    qDebug() << "Kết nối với cổng: " << portname;
    if (!devSerial->open(QIODevice::ReadWrite)) {
        qDebug() << "Không thể mở cổng Serial.";
        return false;
    }

    // Cấu hình các thông số cho cổng Serial
    devSerial->setBaudRate(QSerialPort::Baud9600);
    devSerial->setDataBits(QSerialPort::Data8);
    devSerial->setParity(QSerialPort::NoParity);
    devSerial->setStopBits(QSerialPort::OneStop);
    devSerial->setFlowControl(QSerialPort::NoFlowControl);

    if (devSerial->isOpen()) {
        qDebug() << "Đã kết nối thành công với cổng Serial " << portname;
        return true;
    } else {
        qDebug() << "Kết nối thất bại với cổng Serial " << portname;
        return false;
    }
}

// Phương thức ngắt kết nối với cổng nối tiếp
bool comserial::Disconnect()
{
    devSerial->clear();
    devSerial->close();

    if (devSerial->error() == 0 || !devSerial->isOpen()) {
        qDebug() << "Đóng port thành công";
        return true;
    } else {
        qDebug() << "Ngắt kết nối không thành công! Lỗi: " << devSerial->error();
        return false;
    }
}

// Phương thức ghi dữ liệu xuống cổng nối tiếp
qint64 comserial::Write(const char *cmd)
{
    qint64 tamanhoEscrito;
    tamanhoEscrito = devSerial->write(cmd, qstrlen(cmd));
    return tamanhoEscrito;
}

// Phương thức đọc dữ liệu từ cổng nối tiếp
QString comserial::Read()
{
    QString bufRxSerial;

    // Chờ đọc hết dữ liệu trước khi tiếp tục
    while (devSerial->waitForReadyRead(1000)) {
        bufRxSerial += devSerial->readAll();
    }
    return bufRxSerial;
}

// Phương thức đọc dữ liệu từ cổng nối tiếp với kích thước buffer xác định
QString comserial::Read(int TamanhoBuffer)
{
    char buf[TamanhoBuffer];

    if (devSerial->canReadLine()) {
        devSerial->read(buf, sizeof(buf));
    }

    return buf;
}

// Phương thức gửi dữ liệu xuống cổng nối tiếp
void comserial::sendDataToPort(double dataToSend, QString Str)
{
    if (devSerial->isOpen() && devSerial->isWritable()) {
        // Chuyển đổi giá trị từ double sang QString
        double dataToSendPort = dataToSend;
        QString StrdataToSend = Str + QString::number(dataToSendPort);

        // Chuyển đổi QString sang QByteArray
        QByteArray ArrSendData = StrdataToSend.toUtf8();

        // Gửi dữ liệu xuống thiết bị thông qua cổng nối tiếp
        qint64 bytesWritten = devSerial->write(ArrSendData);
        qDebug() << "Gửi dữ liệu: " << ArrSendData;

        if (bytesWritten == -1) {
            // Xử lý lỗi: Không gửi được dữ liệu
            qDebug() << "Lỗi khi ghi dữ liệu xuống cổng nối tiếp";
        } else if (bytesWritten < ArrSendData.size()) {
            // Xử lý lỗi: Chưa gửi hết dữ liệu
        }
    }
}

