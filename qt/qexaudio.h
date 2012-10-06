#ifndef QEXAUDIO_H
#define QEXAUDIO_H

#include <QObject>
#include <QUrl>
#include <phonon/MediaObject>
#include <QStringList>

class QExAudio : public QObject {
    Q_OBJECT

public:
    explicit QExAudio(QObject *parent = 0);

private:
    Phonon::MediaObject *music;
    int status;
    QUrl nowPlayingUrl;

signals:
    void nativeEvent(QVariantMap event);

public slots:
    void exec(QVariantMap receivedEvent);
    void receiveTick(qint64 tick);
    void receiveFinished();
    void receiveDurationAvailable();
    void receivePlay();
    void receivePause();
    void receiveReady();

protected:


};


#endif // QEXAUDIO_H
