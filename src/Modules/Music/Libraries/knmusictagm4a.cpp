#include <QDataStream>
#include <QFile>

#include <QDebug>

#include <cstdlib>
#include <cstring>

#include "knmusictagm4a.h"

KNMusicTagM4A::KNMusicTagM4A(QObject *parent) :
    KNMusicTagBase(parent)
{
    char rawCharCode[5];
    rawCharCode[4]='\0';
    rawCharCode[0]=0xA9; rawCharCode[1]='a'; rawCharCode[2]='l'; rawCharCode[3]='b';
    m_m4aCharCode[Album]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='A'; rawCharCode[2]='R'; rawCharCode[3]='T';
    m_m4aCharCode[Artist]=QString(rawCharCode);
    rawCharCode[0]='a'; rawCharCode[1]='A'; rawCharCode[2]='R'; rawCharCode[3]='T';
    m_m4aCharCode[AlbumArtist]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='c'; rawCharCode[2]='m'; rawCharCode[3]='t';
    m_m4aCharCode[Comment]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='d'; rawCharCode[2]='a'; rawCharCode[3]='y';
    m_m4aCharCode[Year]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='n'; rawCharCode[2]='a'; rawCharCode[3]='m';
    m_m4aCharCode[Title]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='g'; rawCharCode[2]='e'; rawCharCode[3]='n';
    m_m4aCharCode[Genre]=QString(rawCharCode);
    rawCharCode[0]='t'; rawCharCode[1]='r'; rawCharCode[2]='k'; rawCharCode[3]='n';
    m_m4aCharCode[Tracknumber]=QString(rawCharCode);
    rawCharCode[0]='d'; rawCharCode[1]='i'; rawCharCode[2]='s'; rawCharCode[3]='k';
    m_m4aCharCode[Disknumber]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='w'; rawCharCode[2]='r'; rawCharCode[3]='t';
    m_m4aCharCode[Composer]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='t'; rawCharCode[2]='o'; rawCharCode[3]='o';
    m_m4aCharCode[Encoder]=QString(rawCharCode);
    rawCharCode[0]='t'; rawCharCode[1]='m'; rawCharCode[2]='p'; rawCharCode[3]='o';
    m_m4aCharCode[BPM]=QString(rawCharCode);
    rawCharCode[0]='c'; rawCharCode[1]='p'; rawCharCode[2]='r'; rawCharCode[3]='t';
    m_m4aCharCode[Copyright]=QString(rawCharCode);
    rawCharCode[0]='c'; rawCharCode[1]='p'; rawCharCode[2]='i'; rawCharCode[3]='l';
    m_m4aCharCode[Compilation]=QString(rawCharCode);
    rawCharCode[0]='c'; rawCharCode[1]='o'; rawCharCode[2]='v'; rawCharCode[3]='r';
    m_m4aCharCode[Artwork]=QString(rawCharCode);
    rawCharCode[0]='r'; rawCharCode[1]='t'; rawCharCode[2]='n'; rawCharCode[3]='g';
    m_m4aCharCode[Rating]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='g'; rawCharCode[2]='r'; rawCharCode[3]='p';
    m_m4aCharCode[Grouping]=QString(rawCharCode);
    rawCharCode[0]='s'; rawCharCode[1]='t'; rawCharCode[2]='i'; rawCharCode[3]='k';
    m_m4aCharCode[MediaType]=QString(rawCharCode);
    rawCharCode[0]='p'; rawCharCode[1]='c'; rawCharCode[2]='s'; rawCharCode[3]='t';
    m_m4aCharCode[Podcast]=QString(rawCharCode);
    rawCharCode[0]='c'; rawCharCode[1]='a'; rawCharCode[2]='t'; rawCharCode[3]='g';
    m_m4aCharCode[Category]=QString(rawCharCode);
    rawCharCode[0]='k'; rawCharCode[1]='e'; rawCharCode[2]='y'; rawCharCode[3]='w';
    m_m4aCharCode[Keyword]=QString(rawCharCode);
    rawCharCode[0]='p'; rawCharCode[1]='u'; rawCharCode[2]='r'; rawCharCode[3]='l';
    m_m4aCharCode[PodcastURL]=QString(rawCharCode);
    rawCharCode[0]='e'; rawCharCode[1]='g'; rawCharCode[2]='i'; rawCharCode[3]='d';
    m_m4aCharCode[EpisodeGlobalUniqueID]=QString(rawCharCode);
    rawCharCode[0]='d'; rawCharCode[1]='e'; rawCharCode[2]='s'; rawCharCode[3]='c';
    m_m4aCharCode[Description]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='l'; rawCharCode[2]='y'; rawCharCode[3]='r';
    m_m4aCharCode[Lyrics]=QString(rawCharCode);
    rawCharCode[0]='t'; rawCharCode[1]='v'; rawCharCode[2]='n'; rawCharCode[3]='n';
    m_m4aCharCode[TVNetworkName]=QString(rawCharCode);
    rawCharCode[0]='t'; rawCharCode[1]='v'; rawCharCode[2]='s'; rawCharCode[3]='h';
    m_m4aCharCode[TVShowName]=QString(rawCharCode);
    rawCharCode[0]='t'; rawCharCode[1]='v'; rawCharCode[2]='e'; rawCharCode[3]='n';
    m_m4aCharCode[TVEpisodeNumber]=QString(rawCharCode);
    rawCharCode[0]='t'; rawCharCode[1]='v'; rawCharCode[2]='s'; rawCharCode[3]='n';
    m_m4aCharCode[TVSeason]=QString(rawCharCode);
    rawCharCode[0]='t'; rawCharCode[1]='v'; rawCharCode[2]='e'; rawCharCode[3]='s';
    m_m4aCharCode[TVEpisode]=QString(rawCharCode);
    rawCharCode[0]='p'; rawCharCode[1]='u'; rawCharCode[2]='r'; rawCharCode[3]='d';
    m_m4aCharCode[PurchaseDate]=QString(rawCharCode);
    rawCharCode[0]='p'; rawCharCode[1]='g'; rawCharCode[2]='a'; rawCharCode[3]='p';
    m_m4aCharCode[GaplessPlayback]=QString(rawCharCode);

}

QString KNMusicTagM4A::metaDataName(const int &index) const
{
    if(index<M4AMetaDataCount)
    {
        return m_m4aCharCode[index];
    }
    return QString();
}

QByteArray KNMusicTagM4A::metaData(const QString &name) const
{
    return m_ilstData[name];
}

QByteArray KNMusicTagM4A::metaData(const int &index) const
{
    QString metaName=metaDataName(index);
    if(metaName.isNull())
    {
        return QByteArray();
    }
    return metaData(metaName);
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
    searchIn(rawTagData, 0, rawTagLength-1, true);
    if(m_tagData["meta"].start!=m_tagData["meta"].end)
    {
        searchIn(rawTagData,
                 m_tagData["meta"].start+4,
                 m_tagData["meta"].end);
        if(m_tagData["ilst"].start!=m_tagData["ilst"].end)
        {
            parseMeta(rawTagData,
                      m_tagData["ilst"].start,
                      m_tagData["ilst"].end);
        }
    }
    delete[] rawTagData;
    QByteArray coverImageData=metaData("covr");
    if(!coverImageData.isNull())
    {
        m_albumArt.loadFromData(coverImageData);
    }
    return true;
}

QPixmap KNMusicTagM4A::albumArt() const
{
    return m_albumArt;
}

void KNMusicTagM4A::parseMeta(const char *rawTagData,
                              const int &start,
                              const int &end)
{
    int position=start, rawFrameEnd;
    quint32 tagSize, realSize;
    char *rawFrameData;
    while(position<=end)
    {
        tagSize=((((quint32)rawTagData[position])<<24)  &0b11111111000000000000000000000000)+
                ((((quint32)rawTagData[position+1])<<16)&0b00000000111111110000000000000000)+
                ((((quint32)rawTagData[position+2])<<8) &0b00000000000000001111111100000000)+
                (((quint32)rawTagData[position+3])      &0b00000000000000000000000011111111);
        if(tagSize==0)
        {
            return;
        }
        rawFrameEnd=position+tagSize-1;
        if(rawFrameEnd>end)
        {
            return;
        }
        memcpy(m_idCache, rawTagData+position+4, 4);
        realSize=((((quint32)rawTagData[position+8])<<24)&0b11111111000000000000000000000000)+
                 ((((quint32)rawTagData[position+9])<<16)&0b00000000111111110000000000000000)+
                 ((((quint32)rawTagData[position+10])<<8)&0b00000000000000001111111100000000)+
                 (((quint32)rawTagData[position+11])     &0b00000000000000000000000011111111);
        QByteArray frameData;
        if(realSize==tagSize-8)
        {
            position+=24;
            realSize-=16;
            rawFrameData=new char[realSize];
            memcpy(rawFrameData, rawTagData+position, realSize);
            frameData.append(rawFrameData, realSize);
            m_ilstData[m_idCache]=frameData;
            delete[] rawFrameData;
            position+=realSize;
            continue;
        }
        realSize=rawFrameEnd-position-8;
        rawFrameData=new char[realSize];
        memcpy(rawFrameData, rawTagData+position+8, realSize);
        frameData.append(rawFrameData, realSize);
        m_ilstData[m_idCache]=frameData;
        delete[] rawFrameData;
        position=rawFrameEnd+1;
    }
}

void KNMusicTagM4A::clearCache()
{
    m_tagData.clear();
    m_ilstData.clear();
    m_albumArt=QPixmap();
    m_idCache[4]='\0';
}

void KNMusicTagM4A::searchIn(const char *rawTagData,
                             const int &start,
                             const int &end,
                             bool searchDeep)
{
    quint32 tagSize;
    int position=start, frameEnd;
    while(position<=end)
    {
        tagSize=((((quint32)rawTagData[position])<<24)  &0b11111111000000000000000000000000)+
                ((((quint32)rawTagData[position+1])<<16)&0b00000000111111110000000000000000)+
                ((((quint32)rawTagData[position+2])<<8) &0b00000000000000001111111100000000)+
                (((quint32)rawTagData[position+3])      &0b00000000000000000000000011111111);
        if(tagSize==0)
        {
            return;
        }
        frameEnd=position+tagSize-1;
        if(frameEnd>end)
        {
            return;
        }
        memcpy(m_idCache, rawTagData+position+4, 4);
        tagData currentTag;
        currentTag.start=position+8;
        currentTag.end=frameEnd;
        m_tagData[m_idCache]=currentTag;
        if(searchDeep)
        {
            searchIn(rawTagData, currentTag.start, currentTag.end);
        }
        position=currentTag.end+1;
    }
}
