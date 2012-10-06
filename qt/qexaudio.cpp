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

    // if we have loaded a song and are ready to play
    QObject::connect(music, SIGNAL(stateChanged(Phonon::StoppedState,Phonon::LoadingState)), this, SLOT(receiveReady()));

    // if we change from playing to paused
    // note: when loading a song, phonon goes from stopped to paused before paused to playing.
    // this is a case we shouldn't have to worry about
    QObject::connect(music, SIGNAL(stateChanged(Phonon::PausedState,Phonon::PlayingState)), this, SLOT(receivePause()));

    // if we change from paused to playing or loading to playing
    QObject::connect(music, SIGNAL(stateChanged(Phonon::PlayingState,Phonon::LoadingState)), this, SLOT(receivePause()));
    QObject::connect(music, SIGNAL(stateChanged(Phonon::PlayingState,Phonon::PausedState)), this, SLOT(receivePause()));

}

// slots that receive signals from webkit

void QExAudio::exec(QVariantMap receivedEvent) {

    QString func = receivedEvent["func"].value<QString>();
    QStringList vars = receivedEvent["vars"].value<QStringList>();

    if (func == "play") {
        qDebug() << "got play";
        // play audio, send playing event
        // @todo check that the audio is in a playable state
        music->play();
        QVariantMap event;
        event.insert("name", "play");
        emit nativeEvent(event);
    }

    else if (func == "pause") {
        // pause currently-playing audio, send an event back that the stream has been paused
        // @todo should check state to make sure audio is pausable
        music->pause();
        QVariantMap event;
        event.insert("name", "pause");
        emit nativeEvent(event);
    }

    else if (func == "load") {
        // load our source into the audio player, send an event back that the audio has been loaded
        music->setCurrentSource(nowPlayingUrl);
        QVariantMap event;
        event.insert("name", "load");
        emit nativeEvent(event);
    }

    else if (func == "setSource" && vars.length() > 0) {
        // set our new source, send an event back that the source has been set
        nowPlayingUrl = QUrl(vars[0]);
        QVariantMap event;
        event.insert("name", "sourceSet");
        event.insert("sourceUrl", nowPlayingUrl.toString());
        emit nativeEvent(event);
    }

    else if (func == "nowPlaying" && vars.length() > 0) {
        // now playing business, used in lockscreen on iOS
    }

    else if (func == "seek" && vars.length() > 0) {
        // seek to a time in the stream
        QString seekTime = vars[0];
        QVariantMap event;
        event.insert("event", "timeUpdate");
        event.insert("time", seekTime);
        if (music->state()) {
            // seek takes time in milliseconds
            music->seek(seekTime.toInt()*1000);
            emit nativeEvent(event);
        }
    }
}

// slots that receive signals from Phonon and send nativeEvent signals

void QExAudio::receiveTick(qint64 tick) {
    // update time every half second
    QVariantMap event;
    event.insert("name", "timeupdate");
    event.insert("currentTime", QString::number(tick));
    emit nativeEvent(event);
}

void QExAudio::receiveFinished() {
    // song is ended
    QVariantMap event;
    event.insert("name", "finished");
    emit nativeEvent(event);
}

void QExAudio::receiveDurationAvailable() {
    QVariantMap event;
    event.insert("name", "durationchanged");
    int durationInSeconds = (int) (music->totalTime())/1000;
    event.insert("duration", durationInSeconds);
    emit nativeEvent(event);
}

void QExAudio::receivePlay() {
    QVariantMap event;
    event.insert("name", "play");
    emit nativeEvent(event);
}

void QExAudio::receiveReady() {
    QVariantMap event;
    event.insert("name", "canplay");
    emit nativeEvent(event);
}

void QExAudio::receivePause() {
    QVariantMap event;
    event.insert("name", "pause");
    emit nativeEvent(event);
}
