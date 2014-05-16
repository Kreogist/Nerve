#include "knmusictagbase.h"

KNMusicTagBase::KNMusicTagBase(QObject *parent) :
    QObject(parent)
{
}

void KNMusicTagBase::clearCache()
{
    ;
}

bool KNMusicTagBase::readTag(const QString &filePath)
{
    Q_UNUSED(filePath);
    return false;
}

QString KNMusicTagBase::textData(const int &key) const
{
    Q_UNUSED(key);
}
