#ifndef KNMUSICBACKEND_H
#define KNMUSICBACKEND_H

#include "../../Public/bass/knlibbass.h"

#include <QObject>

class KNMusicBackend : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicBackend(QObject *parent = 0);
    void playFile(const QString &fileName);
    void setVolume(const float &volume);
    KNLibBass *backend();
    void getGraphicData(float *fftData);
    float volume() const;

signals:
    void positionChanged(quint32 position);
    void durationChanged(quint32 duration);
    void finished();
    void stopped();
    void reachEndOfMusic();

public slots:
    void play();
    void pause();
    void setPosition(const int &position);

private:
    KNLibBass *m_backend;
    float m_originalVolume=-1;
};

#endif // KNMUSICBACKEND_H
