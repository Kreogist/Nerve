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
    clearCache();
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

    quint32 rawTagLength;
    mediaData>>rawTagLength;
    rawTagLength-=8;
    char *rawTagData=new char[rawTagLength];
    mediaData.skipRawData(4); //Skip the name.
    mediaData.readRawData(rawTagData, rawTagLength);
    mediaFile.close();
    m_rawData.append(rawTagData, rawTagLength);
    searchIn(rawTagData, 0, rawTagLength-1);
    delete[] rawTagData;
    return true;
}

void KNMusicTagM4A::clearCache()
{
    m_rawData.clear();
    m_idCache[4]='\0';
}

void KNMusicTagM4A::searchIn(const char *rawTagData,
                             const int &start,
                             const int &end)
{
    quint32 tagSize;
    int position=start;
    while(position<end)
    {
        tagSize=((((quint32)rawTagData[position])<<24)  &0b11111111000000000000000000000000)+
                ((((quint32)rawTagData[position+1])<<16)&0b00000000111111110000000000000000)+
                ((((quint32)rawTagData[position+2])<<8) &0b00000000000000001111111100000000)+
                (((quint32)rawTagData[position+3])      &0b00000000000000000000000011111111);
        qDebug()<<tagSize;
        if(position+tagSize>(quint32)end)
        {
            return;
        }
        memcpy(m_idCache, rawTagData+position+4, 4);
        tagData currentTag;
        currentTag.start=position+8;
        currentTag.end=position+tagSize;
        position=currentTag.end+1;
    }
}
