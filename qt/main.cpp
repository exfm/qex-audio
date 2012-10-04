#include <QApplication>
#include <phonon/MediaObject>
#include "qexaudio.h"

#include <QtWebKit>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QExAudio * audioPlayer = new QExAudio();

//    QString url = "http://api.soundcloud.com/tracks/3377639/stream?consumer_key=leL50hzZ1H8tAdKCLSCnw";
//    audioPlayer->src(url);
//    audioPlayer->load();
//    audioPlayer->play();

    QWebView * view = new QWebView();
    view->load(QUrl("qrc:/view.html"));
    view->page()->mainFrame()->addToJavaScriptWindowObject("audioplayer", audioPlayer);
    view->show();

    return app.exec();
}
