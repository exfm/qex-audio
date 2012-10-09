#ifndef QEXFMSERIAL_H
#define QEXFMSERIAL_H

#include <QObject>
#include <qextserialport.h>

class QExfmSerial : public QObject
{
    Q_OBJECT
public:
    explicit QExfmSerial(QObject *parent = 0);

private:
    QextSerialPort * port;
    
signals:
    void next();
    void prev();
    void play();
    void love();
    
public slots:
    void onDataAvailable();
    
};

#endif // QEXFMSERIAL_H
