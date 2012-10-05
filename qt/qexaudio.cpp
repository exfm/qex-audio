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
    emit audioLoaded();
}

void QExAudio::src(QString urlString) {
    nowPlayingUrl = QUrl(urlString);
    emit sourceChanged();
}

void QExAudio::play() {
    music->play();
    emit audioPlaying();
}

void QExAudio::pause() {
    music->pause();
    emit audioPaused();
}

