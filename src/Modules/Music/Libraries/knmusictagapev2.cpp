#include <QFile>

#include <QDebug>

#include <cstdlib>
#include <cstring>

#include "knmusictagapev2.h"

KNMusicTagAPEv2::KNMusicTagAPEv2(QObject *parent) :
    KNMusicTagBase(parent)
{
}

bool KNMusicTagAPEv2::readTag(const QString &filePath)
{
    clearCache();
    QFile mediaFile(filePath);
    if(mediaFile.size()<32)
    {
        //A file even can't contains a header.
        return false;
    }
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);
    if(checkAPEHeaderAt(0, mediaData))
    {
        qDebug()<<"Find at 0";
        return true;
    }
    qDebug()<<"Can't find at 0";
}

void KNMusicTagAPEv2::clearCache()
{
    memset(m_apeHeader, 0, 32);
}

bool KNMusicTagAPEv2::checkAPEHeaderAt(int position,
                                       QDataStream &mediaData)
{
    mediaData.device()->reset();
    mediaData.skipRawData(position);
    mediaData.readRawData(m_apeHeader, 32);
    m_preambleCheck[8]='\0';
    memcpy(m_preambleCheck, m_apeHeader, 8);
    return (strcmp(m_preambleCheck, m_apePreamble)==0);
}
