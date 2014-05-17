#include <QFile>

#include <QDebug>

#include <cstdlib>
#include <cstring>

#include "knmusictagapev2.h"

KNMusicTagAPEv2::KNMusicTagAPEv2(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_frames[Name    ]="Title";
    m_frames[Artist  ]="Artist";
    m_frames[Album   ]="Album";
    m_frames[Comments]="Comment";
    m_frames[Composer]="Composer";
    m_frames[Genre   ]="Genre";
    m_frames[Year    ]="Year";
    m_frames[Track   ]="Track";
}

QString KNMusicTagAPEv2::textData(const int &key) const
{
    return QString(m_frameDatas[m_frames[key]]).simplified();
}

QByteArray KNMusicTagAPEv2::tagRawData(const QString &frameKey) const
{
    return m_frameDatas[frameKey];
}

bool KNMusicTagAPEv2::readTag(const QFile &mediaFile,
                              QDataStream &mediaData)
{
    clearCache();
    if(mediaFile.size()<32)
    {
        //A file even can't contains a header.
        return false;
    }
    m_headerPosition=0;
    if(checkAPEHeaderAt(mediaData))
    {
        m_headerPosition+=32;
        return readTagAt(mediaData);
    }
    m_headerPosition=mediaFile.size()-32;
    if(checkAPEHeaderAt(mediaData))
    {
        m_headerPosition-=(m_tagSize+32);
        return readTagAt(mediaData);
    }
    m_headerPosition=mediaFile.size()-160;
    if(checkAPEHeaderAt(mediaData))
    {
        m_headerPosition-=(m_tagSize+32);
        return readTagAt(mediaData);
    }
    return false;
}

void KNMusicTagAPEv2::clearCache()
{
    memset(m_apeHeader, 0, 32);
    m_frameDatas.clear();
}

bool KNMusicTagAPEv2::checkAPEHeaderAt(QDataStream &mediaData)
{
    mediaData.device()->reset();
    mediaData.skipRawData(m_headerPosition);
    mediaData.readRawData(m_apeHeader, 32);
    m_preambleCheck[8]='\0';
    memcpy(m_preambleCheck, m_apeHeader, 8);
    if(strcmp(m_preambleCheck, m_apePreamble)==0)
    {
        m_versionNumber=(((quint32)m_apeHeader[11]<<24)&0b11111111000000000000000000000000)+
                        (((quint32)m_apeHeader[10]<<16)&0b00000000111111110000000000000000)+
                        (((quint32)m_apeHeader[9] <<8) &0b00000000000000001111111100000000)+
                        ( (quint32)m_apeHeader[8]      &0b00000000000000000000000011111111);
        m_tagSize=(((quint32)m_apeHeader[15]<<24)&0b11111111000000000000000000000000)+
                  (((quint32)m_apeHeader[14]<<16)&0b00000000111111110000000000000000)+
                  (((quint32)m_apeHeader[13]<<8) &0b00000000000000001111111100000000)+
                  ( (quint32)m_apeHeader[12]     &0b00000000000000000000000011111111);
        m_itemCount=((((quint32)m_apeHeader[19])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)m_apeHeader[18])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)m_apeHeader[17]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)m_apeHeader[16])&0b00000000000000000000000011111111);
        m_tagsFlags=((((quint32)m_apeHeader[23])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)m_apeHeader[22])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)m_apeHeader[21]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)m_apeHeader[20])&0b00000000000000000000000011111111);
        return true;
    }
    return false;
}

bool KNMusicTagAPEv2::readTagAt(QDataStream &mediaData)
{
    mediaData.device()->reset();
    mediaData.skipRawData(m_headerPosition);
    char *rawTagData=new char[m_tagSize],
         *frameLabel, *rawFrameData;
    mediaData.readRawData(rawTagData, m_tagSize);

    //All process code here.
    quint32 currentPosition=0,
            currentFrameSize;
    int labelStart, labelEnd, labelLength;
    for(quint32 i=0; i<m_itemCount; i++)
    {
        currentFrameSize=(((quint32)rawTagData[currentPosition+3]<<24)&0b11111111000000000000000000000000)+
                         (((quint32)rawTagData[currentPosition+2]<<16)&0b00000000111111110000000000000000)+
                         (((quint32)rawTagData[currentPosition+1]<<8) &0b00000000000000001111111100000000)+
                         ( (quint32)rawTagData[currentPosition]       &0b00000000000000000000000011111111);
        if(currentFrameSize>m_tagSize)
        {
            break;
        }
        labelStart=currentPosition+8;
        labelEnd=labelStart;
        while(rawTagData[labelEnd]!=0)
        {
            labelEnd++;
        }
        labelLength=labelEnd-labelStart+1;
        frameLabel=new char[labelLength+1];
        rawFrameData=new char[currentFrameSize+1];
        memcpy(frameLabel, rawTagData+labelStart, labelLength);
        memcpy(rawFrameData, rawTagData+labelEnd+1, currentFrameSize);
        rawFrameData[currentFrameSize]='\0';
        QByteArray frameData;
        frameData.append(rawFrameData);
        m_frameDatas[frameLabel]=rawFrameData;
        delete[] rawFrameData;
        delete[] frameLabel;
        currentPosition=labelEnd+currentFrameSize+1;
    }
    //All process code above.

    delete[] rawTagData; //Don't touch this.
    return true;
}
