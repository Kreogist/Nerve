#include "knmusictagbase.h"

KNMusicTagBase::KNMusicTagBase(QObject *parent) :
    QObject(parent)
{
}

void KNMusicTagBase::clearCache()
{
    ;
}

bool KNMusicTagBase::readTag(const QFile &mediaFile,
                             QDataStream &mediaData)
{
    Q_UNUSED(mediaFile);
    Q_UNUSED(mediaData);
    return false;
}

QString KNMusicTagBase::textData(const int &key) const
{
    Q_UNUSED(key);
    return QString();
}
