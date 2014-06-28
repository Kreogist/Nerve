#include "../../Public/bass/knlibbass.h"

#include "../../Public/Base/knmusicbackend.h"

#include <QDebug>

#include "knmusicplayerbackend.h"

KNMusicPlayerBackend::KNMusicPlayerBackend(QObject *parent) :
    QObject(parent)
{
    setBackend(new KNLibBass(this));
}

void KNMusicPlayerBackend::setBackend(KNMusicBackend *backend)
{
    m_backend=backend;
    connect(m_backend, &KNMusicBackend::positionChanged,
            this, &KNMusicPlayerBackend::positionChanged);
    connect(m_backend, &KNMusicBackend::finished,
            this, &KNMusicPlayerBackend::finished);
    connect(m_backend, &KNMusicBackend::stopped,
            this, &KNMusicPlayerBackend::stopped);
}

void KNMusicPlayerBackend::playFile(const QString &fileName)
{
    m_backend->playFile(fileName);
}

void KNMusicPlayerBackend::play()
{
    m_backend->play();
}

void KNMusicPlayerBackend::pause()
{
    m_backend->pause();
}

void KNMusicPlayerBackend::stop()
{
    m_backend->stop();
}

void KNMusicPlayerBackend::setPosition(const float &position)
{
    m_backend->setPosition(position);
}

void KNMusicPlayerBackend::setVolume(const float &volume)
{
    m_backend->setVolume(volume);
}

KNMusicBackend *KNMusicPlayerBackend::backend()
{
    return m_backend;
}

void KNMusicPlayerBackend::getGraphicData(float *fftData)
{
    m_backend->getFFTData(fftData);
}

float KNMusicPlayerBackend::volume() const
{
    return m_backend->volume();
}

float KNMusicPlayerBackend::position() const
{
    return m_backend->position();
}
