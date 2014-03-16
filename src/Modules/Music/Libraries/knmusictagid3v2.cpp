#include <QDataStream>
#include <QPixmap>
#include <QFile>

#include <QDebug>

#include "knmusictagid3v2.h"

QString KNMusicTagID3v2::id3v2String(QByteArray value)
{
    if(value.length()>2)
    {
        //Might be unicode string.
        if(((quint8)(value.at(0))==0xFF && (quint8)(value.at(1))==0xFE) ||
           ((quint8)(value.at(0))==0xFE && (quint8)(value.at(1))==0xFF))
        {
            //Unicode String!
            QByteArray content=value;
            content.remove(0,3);
            return QString::fromUtf8(content).simplified();
        }
    }
    QByteArray content=value;
    content.remove(0,1);
    return QString::fromLocal8Bit(content).simplified();
}

QPixmap KNMusicTagID3v2::id3v2Pixmap(QByteArray value)
{
    QPixmap content;
    content.loadFromData(value, "jpg");
    return content;
}

KNMusicTagID3v2::KNMusicTagID3v2(QObject *parent) :
    KNMusicTagBase(parent)
{
}

bool KNMusicTagID3v2::readTag(const QString &filePath)
{
    m_tagData.version=0;
    m_tagData.revision=0;
    m_tagData.unsynchronisation=false;
    m_tagData.extendedHeader=false;
    m_tagData.experimentalIndicator=false;
    m_tagData.frames.clear();

    QFile mediaFile(filePath);
    if(mediaFile.size()<10)
    {
        //If file is less than ID3v2 header, it can't contains ID3v2 tag.
        return false;
    }
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);
    //Detect ID3v2 header.
    char header[10];
    mediaData.readRawData(header, 10);
    if(header[0]!='I' || header[1]!='D' || header[2]!='3')
    {
        //Can't find 'ID3' from the very beginning.
        return false;
    }
    quint32 tagSize=((quint32)header[6]<<21)+
                    ((quint32)header[7]<<14)+
                    ((quint32)header[8]<<7)+
                    header[9];
    if(mediaFile.size()<((qint64)tagSize+10))
    {
        //File is smaller than the tag says, failed to get.
        mediaFile.close();
        return false;
    }
    m_tagData.version=(int)header[3];
    m_tagData.revision=(int)header[4];
    //Process header: header[5]
    m_tagData.unsynchronisation     =(header[5]&0b10000000);
    m_tagData.extendedHeader        =(header[5]&0b01000000);
    m_tagData.experimentalIndicator =(header[5]&0b00100000);
    char *rawTagData=new char[tagSize];
    mediaData.readRawData(rawTagData, tagSize);
    mediaFile.close();

    //All process code here.
    quint32 rawPosition=0;
    char rawFrameID[5];
    rawFrameID[4]='\0';
    while(rawPosition<tagSize)
    {
        strncpy(rawFrameID, rawTagData+rawPosition, 4);
        if(strlen(rawFrameID)==0)
        {
            //If no tags, means behind of these datas are all '\0'.
            break;
        }
        quint32 frameSize=((((quint32)rawTagData[rawPosition+4])<<24) & 0b11111111000000000000000000000000)+
                ((((quint32)rawTagData[rawPosition+5])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)rawTagData[rawPosition+6]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)rawTagData[rawPosition+7])&0b00000000000000000000000011111111);
        char *rawFrameData=new char[frameSize];
        memcpy(rawFrameData, rawTagData+rawPosition+10, frameSize);
        QByteArray currentFrameData;
        currentFrameData.setRawData(rawFrameData, frameSize);
        qDebug()<<id3v2String(currentFrameData);
        m_tagData.frames[rawFrameID]=currentFrameData;
        rawPosition+=(frameSize+10);
        delete[] rawFrameData;
    }
    //All process code above.
    delete[] rawTagData; //Don't touch this.
    return true;
}

QString KNMusicTagID3v2::frameData(const QString &frameID)
{
    return id3v2String(m_tagData.frames[frameID]);
}
