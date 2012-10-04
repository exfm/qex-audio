#include <QtCore/QCoreApplication>
#include <phonon/MediaObject>
#include "qexaudio.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString url = "http://popplers5.bandcamp.com/download/track?enc=mp3-128&id=810720011&stream=1";

    QExAudio * audioPlayer = new QExAudio();
    audioPlayer->src(url);
    audioPlayer->load();
    audioPlayer->play();

    return a.exec();
}
