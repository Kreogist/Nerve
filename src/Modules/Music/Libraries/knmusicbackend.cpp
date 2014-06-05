#include <QDebug>

#include "knmusicbackend.h"

KNMusicBackend::KNMusicBackend(QObject *parent) :
    QObject(parent)
{
    m_backend=new KNLibBass(this);
    connect(m_backend, &KNLibBass::positionChanged,
            this, &KNMusicBackend::positionChanged);
    connect(m_backend, &KNLibBass::finished,
            this, &KNMusicBackend::reachEndOfMusic);
    connect(m_backend, &KNLibBass::stopped,
            this, &KNMusicBackend::stopped);
    connect(m_backend, &KNLibBass::finished,
            this, &KNMusicBackend::finished);
}

void KNMusicBackend::playFile(const QString &fileName)
{
    m_backend->loadMusic(fileName);
    emit durationChanged(m_backend->duration());
    m_backend->play();
}

void KNMusicBackend::play()
{
    m_backend->play();
}

void KNMusicBackend::pause()
{
    m_backend->pause();
}

void KNMusicBackend::setPosition(const int &position)
{
    m_backend->setPosition(position);
}

void KNMusicBackend::setVolume(const float &volume)
{
    m_backend->setVolume(volume);
}

KNLibBass *KNMusicBackend::backend()
{
    return m_backend;
}

void KNMusicBackend::getGraphicData(float *fftData)
{
    m_backend->getFFTData(fftData);
}

float KNMusicBackend::volume() const
{
    return m_backend->volume();
}
