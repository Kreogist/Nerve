#include <QDebug>

#include "../knmusicglobal.h"

#include "knmusicsearcher.h"

KNMusicSearcher::KNMusicSearcher(QObject *parent) :
    KNLibSearcher(parent)
{
    m_musicGlobal=KNMusicGlobal::instance();
}

int KNMusicSearcher::getType(const QString &suffix)
{
    return m_musicGlobal->getMusicType(suffix);
}
