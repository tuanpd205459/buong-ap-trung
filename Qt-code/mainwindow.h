#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "comserial.h"
#include <QDebug>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void WriteData(const QByteArray data);
    void ReadData();
    void on_pushButtonConnect_clicked();
    void on_pushButtonDisconnect_clicked();
    void updateTemperature(QString sensor_reading);
    void updateHumidity(QString sersor_reading);
    void dayHatch(QString sersor_reading);
    void hourHatch(QString sersor_reading);
    void minHatch(QString sersor_reading);
    void readSerial();
    void on_pushButtonStartHatch_clicked();
    void on_pushButtonStopHatch_clicked();
    void on_pushButtonContinue_clicked();
    void on_pushButtonSetTemp_clicked();
    void on_pushButtonTurnOffBuzzer_clicked();
    void on_pushButtonReset_clicked();
    void refreshGraph();
    void showChart();

private:
    Ui::MainWindow *ui;
    QSerialPort *devserial;
    comserial *procSerial;
    QByteArray serialData;
    QString serialBuffer;
    QString parsed_data;
    QString temperature_value;
    QString humidity_value;
    bool isHatching = true;
};

#endif // MAINWINDOW_H

