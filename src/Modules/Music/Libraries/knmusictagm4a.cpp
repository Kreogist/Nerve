#include <QDataStream>
#include <QFile>

#include <QDebug>

#include <cstdlib>
#include <cstring>

#include "knmusictagm4a.h"

KNMusicTagM4A::KNMusicTagM4A(QObject *parent) :
    KNMusicTagBase(parent)
{
}

bool KNMusicTagM4A::readTag(const QString &filePath)
{
    QFile mediaFile(filePath);
    if(mediaFile.size()<4)
    {
        //Smaller than header size number.
        return false;
    }
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);
    quint32 headerSize;
    mediaData>>headerSize;
    char headerChecker[4];
    mediaData.readRawData(headerChecker, 4);
    if(memcmp(headerChecker, m_m4aHeader, 4)!=0)
    {
        //It's not m4a file.
        mediaFile.close();
        return false;
    }
    headerSize-=8;
    mediaData.skipRawData(headerSize);

    quint32 headerLength;
    mediaData>>headerLength;
    headerLength-=4;
    char *rawTagData=new char[headerLength];
    mediaData.readRawData(rawTagData, headerLength);
    mediaFile.close();
    ;
    delete[] rawTagData;
    return true;
}
