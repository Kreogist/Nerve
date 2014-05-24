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
    m_m4aCharCode[AlbumArtist]="aART";
    rawCharCode[0]=0xA9; rawCharCode[1]='c'; rawCharCode[2]='m'; rawCharCode[3]='t';
    m_m4aCharCode[Comment]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='d'; rawCharCode[2]='a'; rawCharCode[3]='y';
    m_m4aCharCode[Year]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='n'; rawCharCode[2]='a'; rawCharCode[3]='m';
    m_m4aCharCode[Title]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='g'; rawCharCode[2]='e'; rawCharCode[3]='n';
    m_m4aCharCode[Genre]=QString(rawCharCode);
    m_m4aCharCode[Tracknumber]=QString("trkn");
    m_m4aCharCode[Disknumber]=QString("disk");
    rawCharCode[0]=0xA9; rawCharCode[1]='w'; rawCharCode[2]='r'; rawCharCode[3]='t';
    m_m4aCharCode[Composer]=QString(rawCharCode);
    rawCharCode[0]=0xA9; rawCharCode[1]='t'; rawCharCode[2]='o'; rawCharCode[3]='o';
    m_m4aCharCode[Encoder]=QString(rawCharCode);
    m_m4aCharCode[BPM]=QString("tmpo");
    m_m4aCharCode[Copyright]=QString("cprt");
    m_m4aCharCode[Compilation]=QString("cpil");
    m_m4aCharCode[Artwork]=QString("covr");
    m_m4aCharCode[Rating]=QString("rtng");
    rawCharCode[0]=0xA9; rawCharCode[1]='g'; rawCharCode[2]='r'; rawCharCode[3]='p';
    m_m4aCharCode[Grouping]=QString(rawCharCode);
    m_m4aCharCode[MediaType]=QString("stik");
    m_m4aCharCode[Podcast]=QString("pcst");
    m_m4aCharCode[Category]=QString("catg");
    m_m4aCharCode[Keyword]=QString("keyw");
    m_m4aCharCode[PodcastURL]=QString("purl");
    m_m4aCharCode[EpisodeGlobalUniqueID]=QString("egid");
    m_m4aCharCode[Description]=QString("desc");
    rawCharCode[0]=0xA9; rawCharCode[1]='l'; rawCharCode[2]='y'; rawCharCode[3]='r';
    m_m4aCharCode[Lyrics]=QString(rawCharCode);
    m_m4aCharCode[TVNetworkName]=QString("tvnn");
    m_m4aCharCode[TVShowName]=QString("tvsh");
    m_m4aCharCode[TVEpisodeNumber]=QString("tven");
    m_m4aCharCode[TVSeason]=QString("tvsn");
    m_m4aCharCode[TVEpisode]=QString("tves");
    m_m4aCharCode[PurchaseDate]=QString("purd");
    m_m4aCharCode[GaplessPlayback]=QString("pgap");

}

QString KNMusicTagM4A::metaDataName(const int &index) const
{
    return index<M4AMetaDataCount?m_m4aCharCode[index]:QString();
}

QString KNMusicTagM4A::textData(const int &key) const
{
    if(key<M4AMetaDataCount)
    {
        int keyIndex=m_keys.indexOf(m_m4aCharCode[key]);
        return keyIndex==-1?QByteArray():m_data.at(keyIndex);
    }
    return QByteArray();
}

QByteArray KNMusicTagM4A::metaData(const QString &name) const
{
    int keyIndex=m_keys.indexOf(name);
    return keyIndex==-1?QByteArray():m_data.at(keyIndex);
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

QStringList KNMusicTagM4A::keyList() const
{
    return m_keys;
}

QString KNMusicTagM4A::frameTextData(const QString &name) const
{
    return name=="covr"?QString("(Binary Data)"):metaData(name);
}

bool KNMusicTagM4A::readTag(const QFile &mediaFile,
                            QDataStream &mediaData)
{
    clearCache();
    if(mediaFile.size()<4)
    {
        //Smaller than header size number.
        return false;
    }
    quint32 headerSize;
    mediaData>>headerSize;
    char headerChecker[4];
    mediaData.readRawData(headerChecker, 4);
    if(memcmp(headerChecker, m_m4aHeader, 4)!=0)
    {
        //It's not m4a file.
        return false;
    }
    headerSize-=8;
    mediaData.skipRawData(headerSize);

    quint32 rawTagLength;
    mediaData>>rawTagLength;
    rawTagLength-=8;
    m_rawTagData=new char[rawTagLength];
    mediaData.skipRawData(4); //Skip the name.
    mediaData.readRawData(m_rawTagData, rawTagLength);
    //mediaFile.close();
    searchIn(0, rawTagLength-1, true);
    if(m_tagData["meta"].start!=m_tagData["meta"].end)
    {
        searchIn(m_tagData["meta"].start+4,
                 m_tagData["meta"].end);
        if(m_tagData["ilst"].start!=m_tagData["ilst"].end)
        {
            parseMeta(m_tagData["ilst"].start,
                      m_tagData["ilst"].end);
        }
    }
    delete[] m_rawTagData;
    QByteArray coverImageData=metaData("covr");
    if(!coverImageData.isNull())
    {
        m_albumArt.loadFromData(coverImageData);
    }
    return true;
}

QImage KNMusicTagM4A::albumArt() const
{
    return m_albumArt;
}

void KNMusicTagM4A::parseMeta(const int &start,
                              const int &end)
{
    int position=start, rawFrameEnd;
    quint32 tagSize, realSize;
    while(position<=end)
    {
        tagSize=(((quint32)m_rawTagData[position  ]<<24)&0b11111111000000000000000000000000)+
                (((quint32)m_rawTagData[position+1]<<16)&0b00000000111111110000000000000000)+
                (((quint32)m_rawTagData[position+2]<<8 )&0b00000000000000001111111100000000)+
                ( (quint32)m_rawTagData[position+3]     &0b00000000000000000000000011111111);
        if(tagSize==0)
        {
            return;
        }
        rawFrameEnd=position+tagSize-1;
        if(rawFrameEnd>end)
        {
            return;
        }
        m_keys.append(QByteArray(m_rawTagData+position+4, 4));
        realSize=(((quint32)m_rawTagData[position+8 ]<<24)&0b11111111000000000000000000000000)+
                 (((quint32)m_rawTagData[position+9 ]<<16)&0b00000000111111110000000000000000)+
                 (((quint32)m_rawTagData[position+10]<<8 )&0b00000000000000001111111100000000)+
                 ( (quint32)m_rawTagData[position+11]     &0b00000000000000000000000011111111);
        if(realSize==tagSize-8)
        {
            position+=24;
            realSize-=16;
            m_data.append(QByteArray(m_rawTagData+position, realSize));
            position+=realSize;
            continue;
        }
        realSize=rawFrameEnd-position-8;
        m_data.append(QByteArray(m_rawTagData+position+8, realSize));
        position=rawFrameEnd+1;
    }
}

void KNMusicTagM4A::clearCache()
{
    m_tagData.clear();
    m_keys.clear();
    m_data.clear();
    m_albumArt=QImage();
    m_idCache[4]='\0';
}

void KNMusicTagM4A::searchIn(const int &start,
                             const int &end,
                             bool searchDeep)
{
    quint32 tagSize;
    int position=start, frameEnd;
    while(position<=end)
    {
        tagSize=(((quint32)m_rawTagData[position  ]<<24)&0b11111111000000000000000000000000)+
                (((quint32)m_rawTagData[position+1]<<16)&0b00000000111111110000000000000000)+
                (((quint32)m_rawTagData[position+2]<<8 )&0b00000000000000001111111100000000)+
                ( (quint32)m_rawTagData[position+3]     &0b00000000000000000000000011111111);
        if(tagSize==0)
        {
            return;
        }
        frameEnd=position+tagSize-1;
        if(frameEnd>end)
        {
            return;
        }
        memcpy(m_idCache, m_rawTagData+position+4, 4);
        tagData currentTag;
        currentTag.start=position+8;
        currentTag.end=frameEnd;
        m_tagData[m_idCache]=currentTag;
        if(searchDeep)
        {
            searchIn(currentTag.start, currentTag.end);
        }
        position=currentTag.end+1;
    }
}
