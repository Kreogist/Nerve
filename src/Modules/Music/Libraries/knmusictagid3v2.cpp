#include <QDataStream>
#include <QTextCodec>
#include <QFile>

#include <QDebug>

#include "knmusictagid3v2.h"

KNMusicTagID3v2::KNMusicTagID3v2(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_beCodec=QTextCodec::codecForName("UTF-16BE");
    m_leCodec=QTextCodec::codecForName("UTF-16LE");
}

QString KNMusicTagID3v2::id3v2String(const QByteArray &value)
{
    QByteArray content=value;
    quint8 encoding=(quint8)(value.at(0));
    switch(encoding)
    {
    case 0:
        //ISO
        content.remove(0,1);
        return QString::fromLocal8Bit(content).simplified();
    case 1:
        content.remove(0,1);
        if((quint8)content.at(0)==0xFE && (quint8)content.at(1)==0xFF)
        {
            content.remove(0,2);
            return m_beCodec->toUnicode(content).simplified();
        }
        if((quint8)content.at(0)==0xFF && (quint8)content.at(1)==0xFE)
        {
            content.remove(0,2);
            return m_leCodec->toUnicode(content).simplified();
        }
        return QString::fromUtf8(content).simplified();
    default:
        return QString(content).simplified();
    }
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
        IDv2Frame currentFrame;
        currentFrame.frameID=rawFrameID;
        quint32 frameSize=((((quint32)rawTagData[rawPosition+4])<<24)&0b11111111000000000000000000000000)+
                ((((quint32)rawTagData[rawPosition+5])<<16)&0b00000000111111110000000000000000)+
                (((((quint32)rawTagData[rawPosition+6]))<<8)&0b00000000000000001111111100000000)+
                (((quint32)rawTagData[rawPosition+7])&0b00000000000000000000000011111111);
        char *rawFrameData=new char[frameSize];
        memcpy(rawFrameData, rawTagData+rawPosition+10, frameSize);
        currentFrame.data.setRawData(rawFrameData, frameSize);
        m_tagData.frames.append(currentFrame);
        if(currentFrame.frameID=="APIC")
        {
            processAPIC(currentFrame.data);
        }
        rawPosition+=(frameSize+10);
        delete[] rawFrameData;
    }
    //All process code above.
    delete[] rawTagData; //Don't touch this.
    return true;
}

void KNMusicTagID3v2::processAPIC(const QByteArray &value)
{
    ID3v2Image currentImage;
    QByteArray content=value;
    quint8 encoding=(quint8)(value.at(0));
    int zeroCharEnd=content.indexOf('\0', 1);
    QString mimeType(content.mid(1, zeroCharEnd-1)),
            imageType=mimeType.mid(6);
    quint8 pictureType=(quint8)(value.at(zeroCharEnd+1));
    zeroCharEnd+=2;
    int descriptionEnd;
    switch(encoding)
    {
    case 0:
        //ISO
        descriptionEnd=content.indexOf('\0', zeroCharEnd);
        currentImage.description=
            QString::fromLocal8Bit(content.mid(zeroCharEnd, descriptionEnd-zeroCharEnd+1)).simplified();
        content.remove(0, zeroCharEnd+1);
        break;
    default:
        break;
    }
    currentImage.image.loadFromData(content, imageType.toStdString().data());
    m_tagImages[pictureType]=currentImage;
}

QMap<int, KNMusicTagID3v2::ID3v2Image> KNMusicTagID3v2::tagImages() const
{
    return m_tagImages;
}

void KNMusicTagID3v2::setTagImages(const QMap<int, KNMusicTagID3v2::ID3v2Image> &tagImages)
{
    m_tagImages = tagImages;
}
