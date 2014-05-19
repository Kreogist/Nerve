#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicfilter.h"

KNMusicFilter::KNMusicFilter(QObject *parent) :
    KNLibFilter(parent)
{
    m_musicGlobal=KNMusicGlobal::instance();
}

int KNMusicFilter::getType(const QString &suffix)
{
    return m_musicGlobal->getMusicType(suffix);
}
