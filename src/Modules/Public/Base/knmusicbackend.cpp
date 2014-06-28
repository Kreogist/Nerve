#include "knmusicbackend.h"

KNMusicBackend::KNMusicBackend(QObject *parent) :
    QObject(parent)
{
}

QString KNMusicBackend::eqFrequencyTitle(const int &index)
{
    return m_eqTitle[index];
}
