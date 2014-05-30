#ifndef KNMUSICPLAYER_H
#define KNMUSICPLAYER_H

#include <QObject>

class KNLibBass;
class KNMusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlayer(QObject *parent = 0);
    void playFile(const QString &fileName);
    void setVolume(const int &volume);
    int volume() const;

signals:
    void positionChanged(quint32 position);
    void durationChanged(quint32 duration);
    void reachEndOfMusic();

public slots:
    void halfVolume();
    void restoreHalfVolume();

private:
    KNLibBass *m_player;
    int m_originalVolume=-1;
};

#endif // KNMUSICPLAYER_H
