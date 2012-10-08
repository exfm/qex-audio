#ifndef QEXWEBPAGE_H
#define QEXWEBPAGE_H

#include <QWebPage>
#include "qexaudio.h"

class QExWebPage : public QWebPage
{
    Q_OBJECT
public:
    QExWebPage(QObject * parent = 0 );
    QExAudio * audioPlayer;
protected:
    void javaScriptConsoleMessage(const QString& message, int lineNumber, const QString& sourceID);
};

#endif // QEXWEBPAGE_H
