#include <QDataStream>
#include <QTextCodec>
#include <QFile>

#include <QDebug>

#include "knmusictagid3v2.h"

KNMusicTagID3v2::KNMusicTagID3v2(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_utf8Codec=QTextCodec::codecForName("UTF-8");
    m_localeCodec=QTextCodec::codecForLocale();
    m_beCodec=QTextCodec::codecForName("UTF-16BE");
    m_leCodec=QTextCodec::codecForName("UTF-16LE");
}

QString KNMusicTagID3v2::id3v2DataToString(const QByteArray &value) const
{
    QByteArray content=value;
    if(content.at(content.length()-1)==0)
    {
        content.remove(content.length()-1, 1);
    }
    quint8 encoding=(quint8)(value.at(0));
    switch(encoding)
    {
    case 0:
        //ISO, here use UTF-8 instead, because UTF-8 can display ISO.
        content.remove(0,1);
        return m_utf8Codec->toUnicode(content).simplified();
    case 1:
        //UTF-16 LE/BE
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
        return m_localeCodec->toUnicode(content).simplified();
    }
}

int KNMusicTagID3v2::hexTo5Rating(const quint8 &hex) const
{
    if(hex>0 && hex<32)
    {
        return 1;
    }
    if(hex>31 && hex<96)
    {
        return 2;
    }
    if(hex>95 && hex<160)
    {
        return 3;
    }
    if(hex>159 && hex<224)
    {
        return 4;
    }
    if(hex>223)
    {
        return 5;
    }
    return 0;
}

QString KNMusicTagID3v2::id3v2String(const QString &frameID) const
{
    int frameDataIndex=m_tagData.frameID.indexOf(frameID);
    if(frameDataIndex==-1)
    {
        return QString();
    }
    return id3v2DataToString(m_tagData.frameData.at(frameDataIndex));
}

int KNMusicTagID3v2::id3v2DataToRating(const QString &frameID) const
{
    int frameDataIndex=m_tagData.frameID.indexOf(frameID);
    if(frameDataIndex==-1)
    {
        return 0;
    }
    QByteArray ratingRaw=m_tagData.frameData.at(frameDataIndex);
    QString windowsMediaTest=QString(ratingRaw);
    if(windowsMediaTest.indexOf("Windows Media Player")!=-1)
    {
        return hexTo5Rating((quint8)ratingRaw.at(ratingRaw.size()-1));
    }
    return 0;
}

QByteArray KNMusicTagID3v2::id3v2Raw(const QString &frameID) const
{
    int frameDataIndex=m_tagData.frameID.indexOf(frameID);
    if(frameDataIndex==-1)
    {
        return QByteArray();
    }
    return m_tagData.frameData.at(frameDataIndex);
}

void KNMusicTagID3v2::clearCache()
{
    m_tagData.version=0;
    m_tagData.revision=0;
    m_tagData.unsynchronisation=false;
    m_tagData.extendedHeader=false;
    m_tagData.experimentalIndicator=false;
    m_tagData.frameID.clear();
    m_tagData.frameData.clear();
    m_tagImages.clear();
}

int KNMusicTagID3v2::version()
{
    return m_tagData.version;
}

bool KNMusicTagID3v2::readTag(const QString &filePath)
{
    clearCache();
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
    quint32 tagSize=(((quint32)header[6]<<21)&0b00001111111000000000000000000000)+
                    (((quint32)header[7]<<14)&0b00000000000111111100000000000000)+
                    (((quint32)header[8]<<7) &0b00000000000000000011111110000000)+
                    ((quint32)header[9]      &0b00000000000000000000000001111111);
    if(mediaFile.size()<((qint64)tagSize+10))
    {
        //File is smaller than the tag says, failed to get.
        mediaFile.close();
        return false;
    }
    m_tagData.version=(int)header[3];
    bool id3v23=(m_tagData.version>2);
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
    if(id3v23)
    {
        rawFrameID[4]='\0';
        quint32 frameSize;
        while(rawPosition<tagSize)
        {
            strncpy(rawFrameID, rawTagData+rawPosition, 4);
            if(strlen(rawFrameID)==0)
            {
                //If no tags, means behind of these datas are all '\0'.
                break;
            }
            if(m_tagData.version==3)
            {
                frameSize=(((quint32)rawTagData[rawPosition+4]<<24)&0b11111111000000000000000000000000)+
                          (((quint32)rawTagData[rawPosition+5]<<16)&0b00000000111111110000000000000000)+
                          (((quint32)rawTagData[rawPosition+6]<<8) &0b00000000000000001111111100000000)+
                          ( (quint32)rawTagData[rawPosition+7]     &0b00000000000000000000000011111111);

            }
            else
            {
                frameSize=(((quint32)rawTagData[rawPosition+4]<<21)&0b00001111111000000000000000000000)+
                          (((quint32)rawTagData[rawPosition+5]<<14)&0b00000000000111111100000000000000)+
                          (((quint32)rawTagData[rawPosition+6]<<7) &0b00000000000000000011111110000000)+
                          ( (quint32)rawTagData[rawPosition+7]     &0b00000000000000000000000001111111);
            }
            if(frameSize>tagSize)
            {
                //Reach an unexpect frame.
                break;
            }
            char *rawFrameData=new char[frameSize+1];
            memcpy(rawFrameData, rawTagData+rawPosition+10, frameSize);
            rawFrameData[frameSize]='\0';
            QByteArray frameData;
            frameData.append(rawFrameData, frameSize);
            m_tagData.frameID.append(rawFrameID);
            m_tagData.frameData.append(frameData);
            if(QString(rawFrameID)=="APIC")
            {
                processAPIC(frameData);
            }
            rawPosition+=(frameSize+10);
            delete[] rawFrameData;
        }
    }
    else
    {
        rawFrameID[3]='\0';
        while(rawPosition<tagSize)
        {
            strncpy(rawFrameID, rawTagData+rawPosition, 3);
            if(strlen(rawFrameID)==0)
            {
                //If no tags, means behind of these datas are all '\0'.
                break;
            }
            quint32 frameSize=((((quint32)rawTagData[rawPosition+3])<<16)&0b00000000111111110000000000000000)+
                              (((((quint32)rawTagData[rawPosition+4]))<<8)&0b00000000000000001111111100000000)+
                              (((quint32)rawTagData[rawPosition+5])&0b00000000000000000000000011111111);
            if(frameSize>tagSize)
            {
                //Reach an unexpect frame.
                break;
            }
            char *rawFrameData=new char[frameSize];
            memcpy(rawFrameData, rawTagData+rawPosition+6, frameSize);
            QByteArray frameData;
            frameData.append(rawFrameData, frameSize);
            m_tagData.frameID.append(rawFrameID);
            m_tagData.frameData.append(frameData);
            if(QString(rawFrameID)=="PIC")
            {
                processAPIC(frameData);
            }
            rawPosition+=(frameSize+6);
            delete[] rawFrameData;
        }
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
            imageType;
    if(mimeType.left(6).toLower()=="image/")
    {
        imageType=mimeType.mid(6);
    }
    else
    {
        imageType=mimeType;
    }
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
        content.remove(0, descriptionEnd+1);
        break;
    default:
        descriptionEnd=content.indexOf('\0', zeroCharEnd);
        content.remove(0, descriptionEnd+1);
        break;
    }
    currentImage.image.loadFromData(content, imageType.toStdString().data());
    m_tagImages[pictureType]=currentImage;
}

QPixmap KNMusicTagID3v2::tagImage(const int &index) const
{
    return m_tagImages[index].image;
}

QPixmap KNMusicTagID3v2::firstAvaliableImage() const
{
    if(m_tagImages.count()!=0)
    {
        return m_tagImages.first().image;
    }
    return QPixmap();
}
