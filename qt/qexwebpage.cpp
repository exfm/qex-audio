#include "qexwebpage.h"

#include <QDebug>
#include <QFile>00
#include <QTextStream>
#include <QtWebKit>


QExWebPage::QExWebPage(QObject *parent): QWebPage(parent){
    audioPlayer = new QExAudio();
}

void QExWebPage::javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID){
   QString logEntry = message +" on line:"+ QString::number(lineNumber) +" Source:"+ sourceID;
   qDebug()<<logEntry;
}
