#ifndef KNMUSICPLAYER_H
#define KNMUSICPLAYER_H

#include "../../Public/bass/knlibbass.h"

#include <QObject>

class KNMusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlayer(QObject *parent = 0);
    void playFile(const QString &fileName);
    void previewFile(const QString &fileName);
    void setVolume(const float &volume);
    KNLibBass *backend();
    int volume() const;

signals:
    void positionChanged(quint32 position);
    void durationChanged(quint32 duration);
    void reachEndOfMusic();

public slots:

private:
    KNLibBass *m_player;
    float m_originalVolume=-1;
};

#endif // KNMUSICPLAYER_H
