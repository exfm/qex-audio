#include "qexaudio.h"
#include <phonon/MediaObject>
#include <QDebug>

QExAudio::QExAudio(QObject *parent) :
    QObject(parent) {
    // constructor
    music = createPlayer(Phonon::MusicCategory,Phonon::MediaSource(""));

}

void QExAudio::load() {
    music->setCurrentSource(Phonon::MediaSource(nowPlayingUrl));
    qDebug() << nowPlayingUrl;
}

void QExAudio::src(QString urlString) {
    nowPlayingUrl = QUrl(urlString);
    qDebug() << nowPlayingUrl;
}

void QExAudio::play() {
    music->play();
    qDebug() << nowPlayingUrl;

}

void QExAudio::pause() {
    music->pause();
}

