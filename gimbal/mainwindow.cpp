#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"command_format.h"
#include<qwt/qwt_dial_needle.h>
#include<qwt/qwt_dial.h>
#include<QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QDebug>
#include<QTimer>
//#include <QTime>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <math.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_picker.h>

QwtPlotCurve * Roll_Curve;
QwtPlotCurve * Pitch_Curve;
QwtPlotCurve * Yaw_Curve;
double Pitch,Roll,Yaw;

QSerialPort serial;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /**********读取串口端口信息************/
    foreach( const QSerialPortInfo &Info,QSerialPortInfo::availablePorts())//读取串口信息
        {
            qDebug() << "portName:"  << Info.portName();//调试时可以看的串口信息
            qDebug() << "Description:" << Info.description();
            qDebug() << "Manufacturer:" << Info.manufacturer();

            serial.setPort(Info);

            if( serial.open( QIODevice::ReadWrite) )//如果串口是可以读写方式打开的
            {
                ui->comboBox_portName->addItem(Info.portName());//在comboBox那添加串口号
                serial.close();//然后自动关闭等待人为开启（通过那个打开串口的PushButton）
            }
        }
    CompassInit();
    PlotInit();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//实现对串口的数据读取
void MainWindow::ReadSerialPort()
{
   requestData=my_serialPort->readAll();

   if(!requestData.isEmpty())
   {
      //判断功能码分别读取 if(requestData[0]==0x01)

       /********以16进制显示*********/
       QDataStream out(&requestData,QIODevice::ReadWrite);    //将字节数组读入
       while(!out.atEnd())
         {
            qint8 outChar = 0;
            out>>outChar;   //每字节填充一次，直到结束
                   //十六进制的转换
           QString str = QString("%1").arg(outChar & 0xFF,2,16,QLatin1Char('0'));
           ui->ReceivedText->insertPlainText(str.toUpper());//大写
           ui->ReceivedText->insertPlainText(" ");//每发送两个字符后添加一个空格
           ui->ReceivedText->moveCursor(QTextCursor::End);
         }

      Roll=uint2int(((uint16_t)requestData[4]<<8)|requestData[5])/65536.0*360.0;
       ui->Roll_Display->setText(QString::number(Roll));             //将double转换为string
       ui->Dial_Roll->setValue(Roll);
       ui->Thermo_Roll->setValue(Roll);
      Pitch=uint2int(((uint16_t)requestData[6]<<8)|requestData[7])/65536.0*360.0;
        ui->Pitch_Display->setText(QString::number(Pitch));
        ui->Dial_Pitch->setValue(Pitch);
        ui->Thermo_Pitch->setValue(Pitch);
       Yaw=uint2int(((uint16_t)requestData[8]<<8)|requestData[9])/65536.0*360.0;
        ui->Yaw_Display->setText(QString::number(Yaw));
        ui->Dial_Yaw->setValue(Yaw);
        ui->Thermo_Yaw->setValue(Yaw);
         requestData.clear();//清除接收数据
}
}

//实现发送数据帧
void MainWindow::SendSerialPort()
{

    {
        QString str="";
        str=ui->SendText->text().trimmed(); //去除空格
        QByteArray senddata;
        StringToHex(str,senddata);
        my_serialPort->write(senddata);
    }
}
void MainWindow::on_OpenserialPort_clicked()
{
        //对串口进行一些初始化
        my_serialPort = new QSerialPort();
        my_serialPort->setPortName(
                    ui->comboBox_portName->currentText() );
        my_serialPort->open( QIODevice::ReadWrite );
        qDebug() << ui->comboBox_portName->currentText();
        my_serialPort->setBaudRate(ui->comboBox_Bandrate->currentText().toInt());//波特率9600
         qDebug()<<ui->comboBox_Bandrate->currentText().toInt();
        my_serialPort->setDataBits( QSerialPort::Data8 );//数据字节，8字节
        my_serialPort->setParity( QSerialPort::NoParity );//校验，无
        my_serialPort->setFlowControl( QSerialPort::NoFlowControl );//数据流控制,无
        my_serialPort->setStopBits(QSerialPort::OneStop);//一位停止位
        connect( my_serialPort, &QSerialPort::readyRead, this,&MainWindow::ReadSerialPort ) ;
        ui->OpenserialPort->setEnabled(false);
        ui->comboBox_portName->setEnabled(false);
}
void MainWindow::PlotInit()
{
     m_timeAreaWidth=100;
    m_basetime = QDateTime::currentDateTime();
    ui->qwtPlot->resize(800,350);
     ui->qwtPlot->setAutoReplot(true);//自动刷新
    ui->qwtPlot->setTitle("三轴角度实时曲线");//标题
    ui->qwtPlot->setCanvasBackground(Qt::black);//背景
    ui->qwtPlot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);//标签


    QwtPlotGrid *grid_1 = new QwtPlotGrid;//网格
    grid_1->enableX(true);
    grid_1->enableY(true);
    grid_1->setMajorPen(Qt::white,0,Qt::DotLine);
    grid_1->attach( ui->qwtPlot);

    //(void) new QwtPlotMagnifier( ui->qwtPlot->canvas());
   // (void) new QwtPlotPanner( ui->qwtPlot->canvas() ); //使用鼠标左键平移
    //(void) new QwtPlotMagnifier( ui.Hum_qwtPlot->canvas());
    //(void) new QwtPlotPanner( ui.Hum_qwtPlot->canvas() ); //使用鼠标左键平移

    ui->qwtPlot->setAxisTitle(QwtPlot::xBottom,"时间");
     ui->qwtPlot->setAxisTitle(QwtPlot::yLeft,"角度/°");
     ui->qwtPlot->setAxisScaleDraw(QwtPlot::xBottom,new TimeScaleDraw(m_basetime));
     ui->qwtPlot->setAxisScale(QwtPlot::yLeft,-180,180,20);


    Roll_Curve=new QwtPlotCurve();//温度曲线实例化
    Roll_Curve->setTitle("横滚角Roll");//曲线名
    Roll_Curve->setPen(Qt::green,2);//曲线颜色，宽度
   // Roll_Curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//平滑曲线


    Pitch_Curve=new QwtPlotCurve();
    Pitch_Curve->setTitle("俯仰角Pitch");
    Pitch_Curve->setPen(Qt::yellow,2);
   // Pitch_Curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//平滑曲线

    Yaw_Curve=new QwtPlotCurve();
    Yaw_Curve->setTitle("偏航角Yaw");
    Yaw_Curve->setPen(Qt::red,2);
//    Yaw_Curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//平滑曲线

    updateTimer=new QTimer(this);
    updateTimer->start(1000);
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(replotCurves()));

}
/********完成dial控件的初始化工作**********/
void MainWindow::CompassInit()
{ 
    ui->Thermo_Roll->colorMap();
    //ui->Dial_Roll->setGeometry(20,70,  200, 200);
    ui->Dial_Roll->setLowerBound(-180);
    ui->Dial_Roll->setUpperBound(180);
    ui->Dial_Roll->setMode(QwtDial::RotateNeedle);
    ui->Dial_Roll->setValue(0);
     QwtCompassMagnetNeedle*  dial_needle_1 = new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle,Qt::white,Qt::red);;
    ui->Dial_Roll->setNeedle(dial_needle_1);
   // ui->Dial_Pitch->setGeometry(20,70,  200, 200);
    ui->Dial_Pitch->setLowerBound(-180);
    ui->Dial_Pitch->setUpperBound(180);
    ui->Dial_Pitch->setMode(QwtDial::RotateNeedle);
    ui->Dial_Pitch->setValue(0);
    QwtCompassMagnetNeedle*  dial_needle_2 = new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle,Qt::white,Qt::blue);;
    ui->Dial_Pitch->setNeedle(dial_needle_2);
  //  ui->Dial_Yaw->setGeometry(20,70,  200, 200);
    ui->Dial_Yaw->setLowerBound(-180);
    ui->Dial_Yaw->setUpperBound(180);
    ui->Dial_Yaw->setMode(QwtDial::RotateNeedle);
    ui->Dial_Yaw->setValue(0);
    QwtCompassMagnetNeedle*  dial_needle_3 = new QwtCompassMagnetNeedle(QwtCompassMagnetNeedle::TriangleStyle,Qt::white,Qt::green);;
    ui->Dial_Yaw->setNeedle(dial_needle_3);
}

void MainWindow::on_SendButton_clicked()
{

    QString str1=ui->SendText->text();
    str1=str1.trimmed();        //去除空格
    QByteArray senddata;
    StringToHex(str1,senddata);
    my_serialPort->write(senddata);
}

void MainWindow::on_CloseSerialPort_clicked()
{
    serial.close();//关闭串口
    ui->OpenserialPort->setEnabled(true);
    ui->comboBox_portName->setEnabled(true);
}

void MainWindow::StringToHex(QString str, QByteArray & senddata)  //字符串转换成16进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}
char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
//        else return (-1);
        else return ch-ch;//不在0-f范围内的会发送成0
}

int16_t MainWindow::uint2int(uint16_t data)
{
    if(data>32767)
        return (32767-data);
    else
        return data;
}
//QDateTime m_basetime;

void MainWindow::replotCurves()
{
  QPoint Roll_Point,Pitch_Point,Yaw_Point;
  qint64 timeVal = (QDateTime::currentDateTime().toMSecsSinceEpoch()-m_basetime.toMSecsSinceEpoch())/1000;

//qDebug()<<t2;

  if(Roll_Vec.size() >= m_timeAreaWidth)
  {
      Roll_Vec.pop_front();
  }

  if(Pitch_Vec.size()>=m_timeAreaWidth)
  {
      Pitch_Vec.pop_front();
  }

  if(Yaw_Vec.size() >= m_timeAreaWidth)
  {
      Yaw_Vec.pop_front();
  }

  Roll_Point.setX(timeVal);
  Roll_Point.setY(Roll);
  Roll_Vec.append(Roll_Point);

  Pitch_Point.setX(timeVal);
  Pitch_Point.setY(Pitch);
  Pitch_Vec.append(Pitch_Point);

  Yaw_Point.setX(timeVal);
  Yaw_Point.setY(Yaw);
  Yaw_Vec.append(Yaw_Point);

  qint64 k = timeVal;
  for ( int j = 0; j < m_timeAreaWidth; j++ )
  {
      m_timeData[j] = k--;
  }
  ui->qwtPlot->setAxisScale(QwtPlot::xBottom, m_timeData[m_timeAreaWidth - 1 ], m_timeData[0]);

  Roll_Curve->setSamples(Roll_Vec);
  Roll_Curve->attach(ui->qwtPlot) ;
  Pitch_Curve->setSamples(Pitch_Vec);
  Pitch_Curve->attach(ui->qwtPlot);
  Yaw_Curve->setSamples(Yaw_Vec);
  Yaw_Curve->attach(ui->qwtPlot);
}
