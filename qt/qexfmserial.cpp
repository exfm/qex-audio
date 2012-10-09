#include "qexfmserial.h"
#include <qextserialport.h>
#include <QDebug>
#include <QApplication>

QExfmSerial::QExfmSerial(QObject *parent) :
    QObject(parent)
{
    QExfmSerial::port = new QextSerialPort("/dev/ttyUSB0");
    port->setBaudRate(BAUD9600);
    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    bool res = false;
    res = port->open(QIODevice::ReadOnly);

    if(res)
    {
        qDebug("Opened");
    }
    else
    {
        qDebug("Failed to connect");
        qDebug() << port->errorString();
    }
}

void QExfmSerial::onDataAvailable(){
    QByteArray data = port->readLine();
    int button = data.toInt();
    switch (button){
        case 2:
            emit play();
            break;
        case 3:
            emit prev();
            break;
        case 4:
            emit next();
            break;
        case 5:
            emit love();
            break;
    }
}
