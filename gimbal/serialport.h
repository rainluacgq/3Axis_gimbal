#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>

class MainWindow;
class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

signals:

public slots:
private:
    MainWindow* Serial_ui;
};

#endif // SERIALPORT_H
