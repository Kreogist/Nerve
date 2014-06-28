#ifndef KNMUSICPLAYERBACKEND_H
#define KNMUSICPLAYERBACKEND_H

#include <QObject>

class KNMusicBackend;
class KNMusicPlayerBackend : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlayerBackend(QObject *parent = 0);
    void setBackend(KNMusicBackend *backend);
    void playFile(const QString &fileName);
    void setVolume(const float &volume);
    KNMusicBackend *backend();
    void getGraphicData(float *fftData);
    float volume() const;
    float position() const;

signals:
    void positionChanged(quint32 position);
    void durationChanged(quint32 duration);
    void stopped();
    void finished();

public slots:
    void play();
    void pause();
    void stop();
    void setPosition(const float &position);

private:
    KNMusicBackend *m_backend;
    float m_originalVolume=-1;
};

#endif // KNMUSICPLAYERBACKEND_H
