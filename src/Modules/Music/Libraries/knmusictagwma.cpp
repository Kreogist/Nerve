#include <QDebug>

#include <QFile>
#include <QTextCodec>
#include <QByteArray>

#include <cstdlib>
#include <cstring>

#include "knmusictagwma.h"

KNMusicTagWma::KNMusicTagWma(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_utf16Codec=QTextCodec::codecForName("UTF-16");
}

bool KNMusicTagWma::readTag(const QString &filePath)
{
    QFile mediaFile(filePath);
    if(mediaFile.size()<16)
    {
        //If file is less than WMA header, it can't contains ID3v2 tag.
        return false;
    }
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);
    //Detect ID3v2 header.
    char header[16];
    mediaData.readRawData(header, 16);
    if(memcmp(header, m_headerMark, 16)!=0)
    {
        //It's not WMA file.
        return false;
    }
    char rawTagSize[14];
    mediaData.readRawData(rawTagSize, 14);
    quint64 tagSize=((((quint64)rawTagSize[7])<<56)&0b1111111100000000000000000000000000000000000000000000000000000000)+
                    ((((quint64)rawTagSize[6])<<48)&0b0000000011111111000000000000000000000000000000000000000000000000)+
                    ((((quint64)rawTagSize[5])<<40)&0b0000000000000000111111110000000000000000000000000000000000000000)+
                    ((((quint64)rawTagSize[4])<<32)&0b0000000000000000000000001111111100000000000000000000000000000000)+
                    ((((quint64)rawTagSize[3])<<24)&0b0000000000000000000000000000000011111111000000000000000000000000)+
                    ((((quint64)rawTagSize[2])<<16)&0b0000000000000000000000000000000000000000111111110000000000000000)+
                    ((((quint64)rawTagSize[1])<<8) &0b0000000000000000000000000000000000000000000000001111111100000000)+
                    (((quint64)rawTagSize[0])      &0b0000000000000000000000000000000000000000000000000000000011111111);

    char *rawTagData=new char[tagSize];
    mediaData.readRawData(rawTagData, tagSize);
    mediaFile.close();

    //All process here.
    quint64 position=0;
    char standardHeader[17];
    memcpy(standardHeader, rawTagData, 16);
    if(memcmp(standardHeader, m_standardFrame, 16)!=0)
    {
        //Can't find standard tags.
        delete[] rawTagData;
        return false;
    }
    position+=16;
    quint8 titleLength=(quint8)rawTagData[position],
           artistLength=(quint8)rawTagData[position+1],
           copyrightLength=(quint8)rawTagData[position+2],
           commentLength=(quint8)rawTagData[position+3],
           unknownLength=(quint8)rawTagData[position+4];
    char *rawStandardData;
    position+=4;

    rawStandardData=new char[titleLength+1];
    rawStandardData[titleLength]='\0';
    memcpy(rawStandardData, rawTagData+position, titleLength);
    m_wmaTags[WMA_FRAMEID_TITLE]=
            m_utf16Codec->toUnicode(rawStandardData).simplified();
    delete[] rawStandardData;
    position+=titleLength;

    rawStandardData=new char[artistLength+1];
    rawStandardData[artistLength]='\0';
    memcpy(rawStandardData, rawTagData, artistLength);
    m_wmaTags[WMA_FRAMEID_AUTHOR]=rawStandardData;
    delete[] rawStandardData;

    rawStandardData=new char[copyrightLength+1];
    rawStandardData[copyrightLength]='\0';
    memcpy(rawStandardData, rawTagData, copyrightLength);
    m_wmaTags[WMA_FRAMEID_COPYRIGHT]=rawStandardData;
    delete[] rawStandardData;

    rawStandardData=new char[commentLength+1];
    rawStandardData[commentLength]='\0';
    memcpy(rawStandardData, rawTagData, commentLength);
    m_wmaTags[WMA_FRAMEID_DESCRIPTION]=rawStandardData;

    rawStandardData=new char[unknownLength+1];
    rawStandardData[unknownLength]='\0';
    memcpy(rawStandardData, rawTagData, unknownLength);
    m_wmaTags[WMA_FRAMEID_UNKNOWN]=rawStandardData;
    delete[] rawStandardData;

    qDebug()<<m_wmaTags[WMA_FRAMEID_TITLE];
    //All process above.

    delete[] rawTagData;
    return true;
}

void KNMusicTagWma::resetCache()
{
    for(int i=0; i<WMAFrameCount; i++)
    {
        m_wmaTags[i].clear();
    }
}
