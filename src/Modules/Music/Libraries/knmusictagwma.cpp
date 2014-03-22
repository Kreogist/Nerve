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
    m_utf16Codec=QTextCodec::codecForName("UTF-16LE");
}

bool KNMusicTagWma::readTag(const QString &filePath)
{
    resetCache();
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

    //Remove the file header and size of tagSize.
    tagSize-=30;

    char *rawTagData=new char[tagSize];
    mediaData.readRawData(rawTagData, tagSize);
    quint64 tagPosition=0;

    char standardFrameTest[17];
    memcpy(standardFrameTest, rawTagData, 16);
    if(memcmp(standardFrameTest, m_standardFrame, 16)!=0)
    {
        //Can't find standard frame.
        delete[] rawTagData;
        return false;
    }
    tagPosition+=16;

    quint64 stdSize=
            ((((quint64)rawTagData[tagPosition+7])<<56)&0b1111111100000000000000000000000000000000000000000000000000000000)+
            ((((quint64)rawTagData[tagPosition+6])<<48)&0b0000000011111111000000000000000000000000000000000000000000000000)+
            ((((quint64)rawTagData[tagPosition+5])<<40)&0b0000000000000000111111110000000000000000000000000000000000000000)+
            ((((quint64)rawTagData[tagPosition+4])<<32)&0b0000000000000000000000001111111100000000000000000000000000000000)+
            ((((quint64)rawTagData[tagPosition+3])<<24)&0b0000000000000000000000000000000011111111000000000000000000000000)+
            ((((quint64)rawTagData[tagPosition+2])<<16)&0b0000000000000000000000000000000000000000111111110000000000000000)+
            ((((quint64)rawTagData[tagPosition+1])<<8) &0b0000000000000000000000000000000000000000000000001111111100000000)+
            (((quint64)rawTagData[tagPosition])        &0b0000000000000000000000000000000000000000000000000000000011111111);

    quint16 stdItemLength[5];
    int standardItemCounter=Title,
        stdItemIndex[5]={WMA_FRAMEID_TITLE,
                              WMA_FRAMEID_AUTHOR,
                              WMA_FRAMEID_COPYRIGHT,
                              WMA_FRAMEID_DESCRIPTION,
                              WMA_FRAMEID_RATING};

    for(int i=8; i<18; i+=2)
    {
        stdItemLength[standardItemCounter]=
                        (((quint16)rawTagData[tagPosition+i+1]<<8)&0b1111111100000000)+
                        (((quint16)rawTagData[tagPosition+i])     &0b0000000011111111);
        standardItemCounter++;
    }

    tagPosition+=18;
    quint16 stringLength;

    for(int i=0; i<5; i++)
    {
        char *stdItemString=new char[stdItemLength[i]];
        memcpy(stdItemString, rawTagData+tagPosition, stdItemLength[i]);
        stringLength=stdItemLength[i]>2?stdItemLength[i]-2:stdItemLength[i];
        m_wmaTags[stdItemIndex[i]].append(stdItemString, stringLength);
        delete[] stdItemString;
        tagPosition+=stdItemLength[i];
    }

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
