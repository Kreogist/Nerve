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

void KNMusicPlayer::play()
{
    m_player->play();
}

void KNMusicPlayer::pause()
{
    m_player->pause();
}

void KNMusicPlayer::setPosition(const int &position)
{
    m_player->setPosition(position);
}

void KNMusicPlayer::setVolume(const float &volume)
{
    m_player->setVolume(volume);
}

KNLibBass *KNMusicPlayer::backend()
{
    return m_player;
}

void KNMusicPlayer::getGraphicData(float *fftData)
{
    m_player->getFFTData(fftData);
}

int KNMusicPlayer::volume() const
{
    return m_player->volume();
}
