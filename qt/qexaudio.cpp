#include "qexaudio.h"
#include <phonon/MediaObject>
#include <QDebug>

QExAudio::QExAudio(QObject *parent) :
    QObject(parent) {
    // constructor

    // statechanged is newState, oldState
    // null to ready: (1, 0)
    //
    music = createPlayer(Phonon::MusicCategory,Phonon::MediaSource(""));
    music->setTickInterval(500);

    QObject::connect(music, SIGNAL(tick(qint64)), this, SLOT(receiveTick(qint64)));
    QObject::connect(music, SIGNAL(finished()), this, SLOT(receiveFinished()));
}

void QExAudio::sendToNative(QString func, QStringList vars) {

    if (func == "play") {
        // play audio, send playing event
        // @todo check that the audio is in a playable state
        music->play();
        QStringList vars;
        emit nativeEvent("play", vars);
    }

    else if (func == "pause") {
        // pause currently-playing audio, send an event back that the stream has been paused
        // @todo should check state to make sure audio is pausable
        music->pause();
        QStringList vars;
        emit nativeEvent("pause", vars);
    }

    else if (func == "load") {
        // load our source into the audio player, send an event back that the audio has been loaded
        music->setCurrentSource(nowPlayingUrl);
        QStringList vars;
        emit nativeEvent("load", vars);
    }

    else if (func == "setSource" && vars.length() > 0) {
        // set our new source, send an event back that the source has been set
        nowPlayingUrl = QUrl(vars[0]);
        QStringList vars;
        vars << nowPlayingUrl.toString();
        emit nativeEvent("sourceSet", vars);
    }

    else if (func == "nowPlaying" && vars.length() > 0) {
        // now playing business, used in lockscreen on iOS
    }

    else if (func == "seek" && vars.length() > 0) {
        // seek to a time in the stream
        QString seekTime = vars[0];
        QStringList vars;
        vars << seekTime;
        if (music->state()) {
            // seek takes time in milliseconds
            music->seek(seekTime.toInt()*1000);
            emit nativeEvent("timeupdate", vars);
        }
    }

}

void QExAudio::receiveTick(qint64 tick) {
    // update time every half second
    QStringList vars;
    vars << QString::number(tick);
    emit nativeEvent("timeupdate", vars);
}

void QExAudio::receiveFinished() {
    // song is ended
    QStringList vars;
    emit nativeEvent("finished", vars);
}

void QExAudio::receiveDurationAvailable() {
    QStringList vars;
    int durationInSeconds = (int) (music->totalTime())/1000;
    vars << QString::number(durationInSeconds);
    emit nativeEvent("durationchanged", vars)
}
