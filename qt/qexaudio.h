#ifndef QEXAUDIO_H
#define QEXAUDIO_H

#include <QObject>
#include <QUrl>
#include <phonon/MediaObject>

class QExAudio : public QObject {
    Q_OBJECT

public:
    explicit QExAudio(QObject *parent = 0);

private:
    Phonon::MediaObject *music;
    int status;
    QUrl nowPlayingUrl;

signals:
    void playingFinished();
    void audioPlaying();
    void audioPaused();
    void audioLoaded();
    void sourceChanged();

public slots:
    void play();
    void pause();
    void load();
    void src(QString urlString);

protected:


};


#endif // QEXAUDIO_H
