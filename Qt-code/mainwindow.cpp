#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comserial.h"
#include "qcustomplot.h"
#include <QTimer>

const qint8 SECONDS_SHOW_ON_GRAPH = 120;  // Hiển thị 120 giây trên đồ thị

QVector<double> time_axis;
QVector<double> temperature_axis;
static qint64 temp_data_idx = 0;   // Dùng sau này để thiết lập phạm vi trục x
static qint64 startTime;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Khởi tạo QVector trong hàm khởi tạo
    time_axis = QVector<double>();
    temperature_axis = QVector<double>();

    double now = QDateTime::currentSecsSinceEpoch();
    time_axis.append(now);
    temperature_axis.append(30);

    QPixmap pix("C:/Users/admin/Downloads/694px-Logo_Hust.png");
    if (!pix.isNull()) {
        ui->label_pix->setPixmap(pix.scaled(ui->label_pix->size(), Qt::KeepAspectRatio));
    } else {
        qDebug() << "Không thể tải hình ảnh";
    }

    devserial = new QSerialPort(this);

    /* Tạo đối tượng của lớp comserial để thao tác đọc/giữ dữ liệu theo cách của tôi */
    procSerial = new comserial(devserial);

    serialBuffer = "";
    parsed_data = "";
    temperature_value = "";
    humidity_value = "";

    QStringList DispSeriais = procSerial->ConnectDevices();
    ui->comboBoxPort->addItems(DispSeriais);

    if (DispSeriais.length() > 0) {
        ui->pushButtonConnect->setEnabled(true);
        ui->textEditGetData->append("###  Cổng nối tiếp đã sẵn sàng để sử dụng.");
    } else {
        ui->textEditGetData->append("### Không phát hiện thấy cổng nối tiếp!");
    }

    connect(devserial, SIGNAL(readyRead()), this, SLOT(readSerial()));

    // Ẩn biểu đồ khi chương trình khởi động
    // ui->customPlot->setVisible(false);

    // Cấu hình đồ thị
    // Tạo biểu đồ và gán dữ liệu vào
    ui->customPlot->addGraph();
    // Đặt nhãn cho trục x và y
    ui->customPlot->xAxis->setLabel("Thời gian");
    ui->customPlot->yAxis->setLabel("Nhiệt độ");
    QColor color(40, 110, 255);
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->customPlot->graph(0)->setPen(QPen(color.lighter(30)));

    // Cấu hình trục x để hiển thị dạng ngày tháng thay vì số
    QSharedPointer<QCPAxisTickerDateTime> date_time_ticker(new QCPAxisTickerDateTime);
    date_time_ticker->setDateTimeFormat("hh:mm:ss");
    ui->customPlot->xAxis->setTicker(date_time_ticker);

    // Đồng bộ phạm vi trục x và y giữa các trục tương ứng
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // Cho phép người dùng kéo phạm vi trục bằng chuột, phóng to/thu nhỏ bằng bánh xe chuột và chọn biểu đồ bằng cách nhấp chuột
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    // Tạo và cấu hình phần tử tiêu đề
    QCPTextElement *title = new QCPTextElement(ui->customPlot);
    title->setText("Biểu đồ nhiệt độ");
    title->setFont(QFont("sans", 12, QFont::Bold));
    ui->customPlot->plotLayout()->insertRow(0);
    ui->customPlot->plotLayout()->addElement(0, 0, title);

    // Bắt đầu định thời cập nhật đồ thị
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshGraph);
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Hàm gửi dữ liệu đến cổng nối tiếp
void MainWindow::WriteData(const QByteArray data)
{
    procSerial->Write(data);
}

// Xử lý sự kiện nút "Kết nối"
void MainWindow::on_pushButtonConnect_clicked()
{
    bool statusOpenSerial;
    statusOpenSerial = procSerial->ConnectCom(ui->comboBoxPort->currentText());
    if (statusOpenSerial) {
        ui->textEditGetData->append("### Kết nối thành công");
    } else {
        ui->textEditGetData->append("### Kết nối thất bại");
    }
}

// Xử lý sự kiện nút "Ngắt kết nối"
void MainWindow::on_pushButtonDisconnect_clicked()
{
    bool statusCloseSerial;
    statusCloseSerial = procSerial->Disconnect();
    if (statusCloseSerial) {
        ui->textEditGetData->append("### Ngắt kết nối thành công!");
    } else {
        ui->textEditGetData->append("### Ngắt kết nối thất bại.");
    }
}

// Cập nhật giá trị nhiệt độ trên màn hình
void MainWindow::updateTemperature(QString sensor_reading)
{
    ui->lcdNumberTemperature->display(sensor_reading);
}

// Cập nhật giá trị độ ẩm trên màn hình
void MainWindow::updateHumidity(QString sensor_reading)
{
    ui->lcdNumberHumidity->display(sensor_reading);
}

// Cập nhật giá trị số ngày ấp trứng trên màn hình
void MainWindow::dayHatch(QString sensor_reading)
{
    ui->lcdNumberDayHatch->display(sensor_reading);
}

// Cập nhật giá trị số giờ ấp trứng trên màn hình
void MainWindow::hourHatch(QString sensor_reading)
{
    ui->lcdNumberHourHatch->display(sensor_reading);
}

// Cập nhật giá trị số phút ấp trứng trên màn hình
void MainWindow::minHatch(QString sensor_reading)
{
    ui->lcdNumberMinHatch->display(sensor_reading);
}

// Xử lý sự kiện đọc dữ liệu từ cổng nối tiếp
void MainWindow::readSerial()
{
    QByteArray serialData = devserial->readAll(); // Đọc dữ liệu từ cổng serial
    serialBuffer = serialBuffer + QString::fromStdString(serialData.toStdString());
    qDebug() << "dữ liệu nhận được: " << serialBuffer;

    // Kiểm tra điều kiện kết thúc
    if (serialBuffer.endsWith("\r\n")) {
        // Xử lý dữ liệu đã đọc
        QStringList values = serialBuffer.split(',');

        if (values.size() >= 2) {
            QString temperature1 = values[0];
            QString humidity1 = values[1];

            if (values.size() >= 5) {
                QString value1 = values[2];
                QString value2 = values[3];
                QString value3 = values[4];

                if (value1.toInt() == 0 && value2.toInt() == 0 && value3.toInt() == 0) {
                    ui->textEditGetData->append("### Trứng đã nở...");
                    // isHatching = true; // Đánh dấu là đang ấp
                } else {
                    if (isHatching == true) {
                        ui->textEditGetData->append("### Đang ấp...");
                        isHatching = false; // Đánh dấu là còn ấp
                    }
                }

                MainWindow::dayHatch(value1);
                MainWindow::hourHatch(value2);
                MainWindow::minHatch(value3);
            }

            bool ok;
            double temperatureValue = humidity1.toDouble(&ok);
            if (!ok) {
                // Xử lý lỗi chuyển đổi (nếu có)
                qDebug() << "Lỗi: Không thể chuyển đổi nhiệt độ thành số thực.";
                return;
            }

            // Lấy thời gian hiện tại dưới dạng số giây kể từ epoch
            double now = QDateTime::currentSecsSinceEpoch();

            // Thêm thời gian và nhiệt độ vào danh sách
            time_axis.append(now);
            temperature_axis.append(temperatureValue);

            // Giới hạn số phần tử trong mảng để hiển thị trong khoảng thời gian quy định (120 giây)
            qint64 timeLimit = now - SECONDS_SHOW_ON_GRAPH;
            while (!time_axis.isEmpty() && time_axis.first() < timeLimit) {
                time_axis.removeFirst();
                temperature_axis.removeFirst();
            }

            // Cập nhật biểu đồ sau khi có dữ liệu mới
            refreshGraph();
            MainWindow::updateTemperature(temperature1);
            MainWindow::updateHumidity(humidity1);
        } else {
            // Xử lý lỗi hoặc thông báo không đủ phần tử trong chuỗi đầu vào
            qDebug() << "Lỗi: Không đủ phần tử trong chuỗi đầu vào.";
        }

        // Xóa serialBuffer để chuẩn bị cho lần đọc tiếp theo
        serialBuffer.clear();
    }
}

// Xử lý sự kiện nút "Bắt đầu ấp trứng"
void MainWindow::on_pushButtonStartHatch_clicked()
{
    procSerial->comserial::sendDataToPort(ui->spinBoxDayTimeHatch->value(), "dayTimeToHatch");
    procSerial->comserial::sendDataToPort(ui->spinBoxHourTimeHatch->value(), "hourTimeToHatch");
    procSerial->comserial::sendDataToPort(ui->spinBoxMinuteTimeHatch->value(), "minuteTimeToHatch");
    procSerial->comserial::sendDataToPort(1, "prState");
    ui->lcdNumberTotalDay->display(ui->spinBoxDayTimeHatch->value());
    ui->lcdNumberTotalHour->display(ui->spinBoxHourTimeHatch->value());
    ui->lcdNumberTotalMin->display(ui->spinBoxMinuteTimeHatch->value());
    ui->textEditGetData->append("### Bắt đầu");
}

// Xử lý sự kiện nút "Dừng ấp trứng"
void MainWindow::on_pushButtonStopHatch_clicked()
{
    procSerial->comserial::sendDataToPort(0, "prState");
    ui->textEditGetData->append("### Dừng");
}

// Xử lý sự kiện nút "Tiếp tục ấp trứng"
void MainWindow::on_pushButtonContinue_clicked()
{
    procSerial->comserial::sendDataToPort(1, "prState");
    ui->textEditGetData->append("### Tiếp tục");
}

// Xử lý sự kiện nút "Cài đặt nhiệt độ"
void MainWindow::on_pushButtonSetTemp_clicked()
{
    int setTemperature = ui->spinBoxSetTemp->value(); // Lấy giá trị từ QSpinBox
    procSerial->comserial::sendDataToPort(setTemperature, "setTemp");
    ui->textEditGetData->append("### Nhiệt độ cài đặt: " + QString::number(setTemperature));
}

// Xử lý sự kiện nút "Tắt còi"
void MainWindow::on_pushButtonTurnOffBuzzer_clicked()
{
    procSerial->comserial::sendDataToPort(2, "TurnOffBuzzer");
    ui->textEditGetData->append("### Tắt còi...");
}

// Xử lý sự kiện nút "Reset"
void MainWindow::on_pushButtonReset_clicked()
{
    procSerial->comserial::sendDataToPort(0, "Reset");
    dayHatch(0);
    hourHatch(0);
    minHatch(0);
    ui->lcdNumberTotalDay->display(0);
    ui->lcdNumberTotalHour->display(0);
    ui->lcdNumberTotalMin->display(0);
    ui->textEditGetData->append("### Reset");
}

// Cập nhật đồ thị
void MainWindow::refreshGraph()
{
    // Kiểm tra giá trị của time_axis và temperature_axis trước khi cập nhật đồ thị
    qint64 timeLimit = QDateTime::currentSecsSinceEpoch() - 20;

    // Loại bỏ các điểm dữ liệu cũ hơn giới hạn thời gian
    while (!time_axis.isEmpty() && time_axis.first() < timeLimit) {
        time_axis.removeFirst();
        temperature_axis.removeFirst();
    }

    // Cập nhật dữ liệu lên biểu đồ
    ui->customPlot->graph()->setData(time_axis, temperature_axis, 1);

    // Điều chỉnh lại phạm vi trục x và y để phù hợp với dữ liệu mới
    ui->customPlot->xAxis->setRange(time_axis.first(), time_axis.last());
    ui->customPlot->yAxis->setRange(temperature_axis.first(), temperature_axis.last());

    // Đảm bảo trục y hiển thị trong khoảng từ 0 đến 100
    ui->customPlot->yAxis->setRange(0, 100);
    ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);

    // Vẽ lại biểu đồ
    ui->customPlot->replot();
}

// Hiển thị biểu đồ
void MainWindow::showChart()
{
    // Xác định loại biểu đồ muốn hiển thị (biểu đồ đường trong trường hợp này)
    ui->customPlot->graph(0)->setLineStyle(QCPGraph::lsLine);

    // Cập nhật biểu đồ với dữ liệu đã có
    refreshGraph();
}

