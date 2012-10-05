#include "qexaudio.h"
#include <phonon/MediaObject>
#include <QDebug>

QExAudio::QExAudio(QObject *parent) :
    QObject(parent) {
    // constructor
    music = createPlayer(Phonon::MusicCategory,Phonon::MediaSource(""));
    music->setTickInterval(500);

    QObject::connect(music, SIGNAL(tick(qint64)), this, SLOT(receiveTick(qint64)));
    QObject::connect(music, SIGNAL(finished()), this, SLOT(receiveFinished()));

}

void QExAudio::sendToNative(QString func, QStringList vars) {

    if (func == "play") {
        music->play();
        QStringList vars;
        emit nativeEvent("play", vars);
    }
    else if (func == "pause") {
        music->pause();
        QStringList vars;
        emit nativeEvent("pause", vars);
    }
    else if (func == "load") {
        music->setCurrentSource(nowPlayingUrl);
        QStringList vars;
        emit nativeEvent("load", vars);
    }
    else if (func == "src" && vars.length() > 0) {
        nowPlayingUrl = QUrl(vars[0]);
        QStringList vars;
        vars << nowPlayingUrl.toString();
        emit nativeEvent("src", vars);
    }
}

void QExAudio::receiveTick(qint64 tick) {
    QStringList vars;
    vars << QString::number(tick);
    emit nativeEvent("tick", vars);
}

void QExAudio::receiveFinished() {
    QStringList vars;
    emit nativeEvent("finished", vars);
}
