#include <QDebug>

#include "../../Public/bass/knlibbass.h"

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

void KNMusicPlayer::setVolume(const int &volume)
{
    m_player->setVolume(volume);
}

int KNMusicPlayer::volume() const
{
    return m_player->volume();
}

void KNMusicPlayer::halfVolume()
{
    m_originalVolume=volume();
    int previewVolume=m_originalVolume>>3;
    m_player->setVolume(previewVolume==0?1:previewVolume);
}

void KNMusicPlayer::restoreHalfVolume()
{
    if(m_originalVolume==-1)
    {
        return;
    }
    m_player->setVolume(m_originalVolume);
    m_originalVolume=-1;
}
