#include "qexaudio.h"
#include <phonon/MediaObject>

QExAudio::QExAudio(QObject *parent) :
    QObject(parent) {
    // constructor
    music = createPlayer(Phonon::MusicCategory,Phonon::MediaSource(""));

}

void QExAudio::load() {
    music->setCurrentSource(Phonon::MediaSource(nowPlayingUrl));
}

void QExAudio::src(QString urlString) {
    nowPlayingUrl = QUrl(urlString);
}

void QExAudio::play() {
    music->play();

}

void QExAudio::pause() {
    music->pause();
}

