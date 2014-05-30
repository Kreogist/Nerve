#include <QDebug>

#include "knmusicplayer.h"

KNMusicPlayer::KNMusicPlayer(QObject *parent) :
    QObject(parent)
{
    m_player=new KNLibBass(this);
    connect(m_player, &KNLibBass::positionChanged,
            this, &KNMusicPlayer::positionChanged);
    /*connect(m_player, &KNLibQtAV::reachEndOfMedia,
            this, &KNMusicPlayer::reachEndOfMusic);*/
}

void KNMusicPlayer::playFile(const QString &fileName)
{
    m_player->loadMusic(fileName);
    emit durationChanged(m_player->duration());
    m_player->play();
}

void KNMusicPlayer::setVolume(const float &volume)
{
    m_player->setVolume(volume);
}

KNLibBass *KNMusicPlayer::backend()
{
    return m_player;
}

int KNMusicPlayer::volume() const
{
    return m_player->volume();
}
