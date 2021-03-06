#include <QDebug>

#include <QFile>
#include <QTextCodec>

#include <cstdlib>
#include <cstring>

#include "knmusictagwma.h"

KNMusicTagWMA::KNMusicTagWMA(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_utf16leCodec=QTextCodec::codecForName("UTF-16LE");
    m_utf16beCodec=QTextCodec::codecForName("UTF-16BE");

    m_frames[Name           ]="WMA_FRAMEID_TITLE";
    m_frames[Artist         ]="WMA_FRAMEID_AUTHOR";
    m_frames[Copyright      ]="WMA_FRAMEID_COPYRIGHT";
    m_frames[Description    ]="WMA_FRAMEID_DESCRIPTION";
    m_frames[Rating         ]="WMA_FRAMEID_RATING";
    m_frames[AlbumArtist    ]="WM/AlbumArtist";
    m_frames[Album          ]="WM/AlbumTitle";
    m_frames[BeatsPerMinuate]="WM/BeatsPerMinute";
    m_frames[Comments       ]="WM/Text";
    m_frames[Composer       ]="WM/Composer";
    m_frames[Genre          ]="WM/Genre";
    m_frames[Year           ]="WM/Year";
    m_frames[TrackNumber    ]="WM/TrackNumber";
}

bool KNMusicTagWMA::readTag(const QFile &mediaFile,
                            QDataStream &mediaData)
{
    clearCache();
    //QFile mediaFile(filePath);
    if(mediaFile.size()<16)
    {
        //If file is less than WMA header, it can't contains ID3v2 tag.
        return false;
    }
    /*if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);*/
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
    quint64 tagSize=(((quint64)rawTagSize[7]<<56)&0b1111111100000000000000000000000000000000000000000000000000000000)+
                    (((quint64)rawTagSize[6]<<48)&0b0000000011111111000000000000000000000000000000000000000000000000)+
                    (((quint64)rawTagSize[5]<<40)&0b0000000000000000111111110000000000000000000000000000000000000000)+
                    (((quint64)rawTagSize[4]<<32)&0b0000000000000000000000001111111100000000000000000000000000000000)+
                    (((quint64)rawTagSize[3]<<24)&0b0000000000000000000000000000000011111111000000000000000000000000)+
                    (((quint64)rawTagSize[2]<<16)&0b0000000000000000000000000000000000000000111111110000000000000000)+
                    (((quint64)rawTagSize[1]<<8) &0b0000000000000000000000000000000000000000000000001111111100000000)+
                    ( (quint64)rawTagSize[0]     &0b0000000000000000000000000000000000000000000000000000000011111111);

    //Remove the file header and size of tagSize.
    tagSize-=30;

    char *rawTagData=new char[tagSize];
    mediaData.readRawData(rawTagData, tagSize);

    quint64 tagPosition=0;
    char frameTest[17];
    quint64 frameSize;

    while(tagPosition<tagSize)
    {
        memcpy(frameTest, rawTagData+tagPosition, 16);
        if(memcmp(frameTest, m_standardFrame, 16)==0)
        {
            tagPosition+=16;

            quint16 stdItemLength[5];
            int standardItemCounter=Name;

            for(int i=8; i<18; i+=2)
            {
                stdItemLength[standardItemCounter]=
                                (((quint16)rawTagData[tagPosition+i+1]<<8)&0b1111111100000000)+
                                (((quint16)rawTagData[tagPosition+i])     &0b0000000011111111);
                standardItemCounter++;
            }
            tagPosition+=18;

            for(int i=0; i<5; i++)
            {
                m_keys.append(m_frames[i]);
                m_data.append(QByteArray(rawTagData+tagPosition,
                                         stdItemLength[i]>2?stdItemLength[i]-2:stdItemLength[i]));
                tagPosition+=stdItemLength[i];
            }
            continue;
        }
        if(memcmp(frameTest, m_extendedFrame, 16)==0)
        {
            tagPosition+=16;
            //This extSize might be no used, too.
            quint16 extFrames=(((quint16)rawTagData[tagPosition+9]<<8)&0b1111111100000000)+
                              (((quint16)rawTagData[tagPosition+8])   &0b0000000011111111);
            tagPosition+=10;

            quint16 rawNameLength, dataLength;
            while(extFrames--)
            {
                rawNameLength=(((quint16)rawTagData[tagPosition+1]<<8)&0b1111111100000000)+
                              (((quint16)rawTagData[tagPosition])     &0b0000000011111111);
                m_keys.append(m_utf16leCodec->toUnicode(rawTagData+tagPosition+2,
                                                        rawNameLength>2?rawNameLength-2:rawNameLength));
                tagPosition+=rawNameLength;
                dataLength=(((quint16)rawTagData[tagPosition+5]<<8)&0b1111111100000000)+
                           (((quint16)rawTagData[tagPosition+4])   &0b0000000011111111);
                tagPosition+=6;
                m_data.append(QByteArray(rawTagData+tagPosition, dataLength));
                tagPosition+=dataLength;
            }
            continue;
        }
        frameSize=(((quint64)rawTagData[tagPosition+23]<<56)&0b1111111100000000000000000000000000000000000000000000000000000000)+
                  (((quint64)rawTagData[tagPosition+22]<<48)&0b0000000011111111000000000000000000000000000000000000000000000000)+
                  (((quint64)rawTagData[tagPosition+21]<<40)&0b0000000000000000111111110000000000000000000000000000000000000000)+
                  (((quint64)rawTagData[tagPosition+20]<<32)&0b0000000000000000000000001111111100000000000000000000000000000000)+
                  (((quint64)rawTagData[tagPosition+19]<<24)&0b0000000000000000000000000000000011111111000000000000000000000000)+
                  (((quint64)rawTagData[tagPosition+18]<<16)&0b0000000000000000000000000000000000000000111111110000000000000000)+
                  (((quint64)rawTagData[tagPosition+17]<<8) &0b0000000000000000000000000000000000000000000000001111111100000000)+
                  ( (quint64)rawTagData[tagPosition+16]     &0b0000000000000000000000000000000000000000000000000000000011111111);
        tagPosition+=frameSize;
    }
    delete[] rawTagData;

    if(m_keys.contains("WM/Picture"))
    {
        processPicture();
    }
    return true;
}

QString KNMusicTagWMA::textData(const int &key) const
{
    int keyIndex=m_keys.indexOf(m_frames[key]);
    if(keyIndex==-1)
    {
        return QString();
    }
    QByteArray contents=m_data.at(keyIndex);
    int contentsSize=contents.size();
    if(contentsSize<2)
    {
        return QString();
    }
    if(contents.at(contentsSize-1)==0 &&
       contents.at(contentsSize-2)==0)
    {
        contents.remove(contentsSize-2, 2);
    }
    return m_utf16leCodec->toUnicode(contents);
}

QString KNMusicTagWMA::frameData(const QString &frame) const
{
    if(frame=="WM/Picture")
    {
        return QString("(Binary Data)");
    }
    int keyIndex=m_keys.indexOf(frame);
    if(keyIndex==-1)
    {
        return QString();
    }
    QByteArray contents=m_data.at(keyIndex);
    int contentsSize=contents.size();
    if(contentsSize<2)
    {
        return QString();
    }
    if(contents.at(contentsSize-1)==0 &&
       contents.at(contentsSize-2)==0)
    {
        contents.remove(contentsSize-2, 2);
    }
    return m_utf16leCodec->toUnicode(contents);
}

QStringList KNMusicTagWMA::keyList() const
{
    return m_keys;
}

QImage KNMusicTagWMA::albumArt() const
{
    return m_albumArt;
}

void KNMusicTagWMA::processPicture()
{
    QByteArray content=m_data.at(m_keys.indexOf("WM/Picture")),
               dblZero, mimeTypeData;
    dblZero.append('\0');
    dblZero.append('\0');
    int mimeStart=content.indexOf('\0', 1)+1,
        mimeEnd=content.indexOf(dblZero, mimeStart),
        descriptionEnd=content.indexOf(dblZero, mimeEnd+2);
    mimeTypeData=content.mid(mimeStart, mimeEnd-mimeStart);
    content.remove(0, descriptionEnd+3);
    m_albumArt.loadFromData(content, "gif");
}

void KNMusicTagWMA::clearCache()
{
    m_keys.clear();
    m_data.clear();
    m_albumArt=QImage();
}
