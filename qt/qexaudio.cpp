#include "qexaudio.h"
#include <phonon/MediaObject>
#include <QDebug>

QExAudio::QExAudio(QObject *parent) :
    QObject(parent) {
    // constructor
    qDebug() << "QExAudio instantiated.";
    this->nowPlayingUrl = "";
    this->canPlay = false;
}

// slots that receive signals from webkit

void QExAudio::exec(QVariantMap receivedEvent) {

    QString func = receivedEvent["func"].value<QString>();
    QStringList vars = receivedEvent["vars"].value<QStringList>();

    if (func == "init") {
        QExAudio::init();
    }

    if (func == "play") {
        // play audio, send playing event
        // @todo check that the audio is in a playable state
        qDebug() << "QExAudio: got play event from client";
//        this->music->setCurrentSource(this->nowPlayingUrl);
        this->music->play();
        qDebug() << "Current state: " << this->music->state();
        qDebug() << "total time: " << this->music->totalTime();

    }

    else if (func == "pause") {
        // pause currently-playing audio, send an event back that the stream has been paused
        // @todo should check state to make sure audio is pausable
        if (this->music->state() == Phonon::PlayingState) {
           this->music->pause();
        }
        qDebug() << "QExAudio pause event";
        QVariantMap event;
        event.insert("name", "pause");
        emit nativeEvent(event);

        qDebug() << "Current state: " << this->music->state();


    }

    else if (func == "load") {
        // load our source into the audio player, send an event back that the audio has been loaded
        this->music->setCurrentSource(this->nowPlayingUrl);
        qDebug() << "QExAudio: received load event";
        QVariantMap event;
        event.insert("name", "load");
        emit nativeEvent(event);

        qDebug() << "Current state: " << this->music->state();



    }

    else if (func == "setSource" && vars.length() > 0) {
        // set our new source, send an event back that the source has been set

        bool wasPlaying = this->music->state() == Phonon::PlayingState;

        this->music->stop();
        this->music->clearQueue();

        this->nowPlayingUrl = QUrl(vars[0]);
        this->music->setCurrentSource(this->nowPlayingUrl);

        qDebug() << "QExAudio: got set source event from client";
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
        if (this->music->state()) {
            // seek takes time in milliseconds
            this->music->seek(seekTime.toInt()*1000);
            emit nativeEvent(event);
        }
    }

    else if (func == "clear") {
        this->music->clear();
    }
}

// slots that receive signals from Phonon and send nativeEvent signals

void QExAudio::receiveTick(qint64 tick) {
    // update time every half second
    QVariantMap event;
    int timeUpdate = (int) tick/1000;
    event.insert("name", "timeupdate");
    event.insert("currentTime", QString::number(timeUpdate));
    emit nativeEvent(event);
}

void QExAudio::receiveFinished() {
    // song is ended
    QVariantMap event;
    event.insert("name", "finished");
    emit nativeEvent(event);
}

void QExAudio::receiveDurationAvailable(qint64 duration) {
    qDebug() << "duration: " << duration;
    QVariantMap event;
    event.insert("name", "durationchanged");
    int durationInSeconds = (int) (duration)/1000;
    event.insert("duration", durationInSeconds);
    emit nativeEvent(event);
}

void QExAudio::receiveStateChange(Phonon::State newState, Phonon::State oldState) {
    qDebug() << "State change from " << oldState << "to " << newState;
    QVariantMap event;
    if (oldState == Phonon::LoadingState) {
        if (newState == Phonon::StoppedState) {
            // loading to ready
            qDebug() << "Emitting canplay event";
            event.insert("name", "canplay");
            emit nativeEvent(event);
        }
        else if (newState == Phonon::PausedState) {
            // loading to ready
            event.insert("name", "canplay");
            emit nativeEvent(event);
        }
    }
    else if (oldState == Phonon::StoppedState) {
        if (newState == Phonon::PlayingState) {
            // stopped to playing
            qDebug() << "Emitting playing event";
            event.insert("name", "playing");
            emit nativeEvent(event);
        }
    }
    else if (oldState == Phonon::PausedState) {
        if (newState == Phonon::PlayingState) {
            // paused to playing
            qDebug() << "Emitting playing event";
            QVariantMap event;
            event.insert("name", "play");
            emit nativeEvent(event);

            event.clear();
            event.insert("name", "playing");
            emit nativeEvent(event);
        }
        else if (newState == Phonon::StoppedState) {
            // loading to ready
            if (this->music->totalTime() > 0) {
                qDebug() << "Emitting canplay event";
                event.insert("name", "canplay");
                emit nativeEvent(event);
            }

        }
    }
    else if (oldState == Phonon::PlayingState) {
        if (newState == Phonon::PausedState) {
            // playing to paused
            event.insert("name", "pause");
            emit nativeEvent(event);
        }
    }
}

void QExAudio::receiveBufferChanged(int percentFilled) {

    QVariantMap event;
    QVariantList ranges;
    QVariantMap range;

    float durationAvailable = (float) ((this->music->totalTime()) / (float)1000) * ((float) percentFilled / (float) 100);

    range.insert("start", 0);
    range.insert("duration", durationAvailable);
    ranges.insert(0, range);
    event.insert("name", "progress");
    event.insert("ranges", ranges);
    emit nativeEvent(event);
}


void QExAudio::init() {
        qDebug() << "init";

        this->music = createPlayer(Phonon::MusicCategory,Phonon::MediaSource(this->nowPlayingUrl));
        this->music->setTickInterval(500);

        QObject::connect(this->music, SIGNAL(tick(qint64)), this, SLOT(receiveTick(qint64)));
        QObject::connect(this->music, SIGNAL(finished()), this, SLOT(receiveFinished()));

        // state change event
        QObject::connect(this->music, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(receiveStateChange(Phonon::State,Phonon::State)));

        // progress
        QObject::connect(this->music, SIGNAL(bufferStatus(int)), this, SLOT(receiveBufferChanged(int)));

        // duration
        QObject::connect(this->music, SIGNAL(totalTimeChanged(qint64)), this, SLOT(receiveDurationAvailable(qint64)));

}
