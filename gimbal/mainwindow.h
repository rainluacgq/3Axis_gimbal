#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QObject>
#include <QMainWindow>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<qwt/qwt_dial.h>
#include<qwt/qwt_dial_needle.h>
#include<qiodevice.h>
#include"timescaledraw.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_OpenserialPort_clicked();

    void on_SendButton_clicked();

    void ReadSerialPort();//定义读取串口的信号槽

    void SendSerialPort();//定义发送数据的信号槽
    void on_CloseSerialPort_clicked();
    void replotCurves();

private:
    void StringToHex(QString str, QByteArray & senddata);
    char ConvertHexChar(char ch) ;
    void CompassInit();
    void PlotInit();

private:
    Ui::MainWindow *ui;
    QSerialPort *my_serialPort;//(实例化一个指向串口的指针，可以用于访问串口)
    QByteArray requestData;//（用于存储从串口那读取的数据）
    QTimer *timer;//（用于计时）
    QDateTime m_basetime;
    double m_timeData[100];
    int m_timeAreaWidth;
    QTimer* updateTimer;
    QVector<QPointF>Roll_Vec,Pitch_Vec,Yaw_Vec;

private:
   int16_t uint2int(uint16_t data);
};

#endif // MAINWINDOW_H
