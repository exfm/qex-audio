#ifndef QEXWEBAPPLICATION_H
#define QEXWEBAPPLICATION_H

#include <QObject>
#include <phonon/MediaObject>
#include "qexaudio.h"
#include <qexwebpage.h>

#include <QtWebKit>

class QExWebApplication : public QObject
{
    Q_OBJECT
public:
    explicit QExWebApplication(QObject *parent = 0);
    QWebView * view;
    QExAudio * audioPlayer;
    void loadAndShow(QString url);
    
signals:
    
public slots:
    void addJavaScriptObject();
};

#endif // QEXWEBAPPLICATION_H
