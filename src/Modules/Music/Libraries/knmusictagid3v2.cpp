#include <QDataStream>
#include <QTextCodec>
#include <QFile>
#include <QLocale>

#include <QDebug>

#include "../../knglobal.h"

#include "knmusictagid3v2.h"

KNMusicTagID3v2::KNMusicTagID3v2(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_isoCodec=QTextCodec::codecForName("GB18030");
    m_beCodec=QTextCodec::codecForName("UTF-16BE");
    m_leCodec=QTextCodec::codecForName("UTF-16LE");
    m_utf16Codec=QTextCodec::codecForName("UTF-16");
    m_utf8Codec=QTextCodec::codecForName("UTF-8");
    m_localCodec=KNGlobal::instance()->codecForCurrentLocale();

    m_frames[Name           ][0]="TIT2";
    m_frames[Artist         ][0]="TPE1";
    m_frames[Album          ][0]="TALB";
    m_frames[AlbumArtist    ][0]="TPE2";
    m_frames[BeatsPerMinuate][0]="TBPM";
    m_frames[Category       ][0]="TIT1";
    m_frames[Comments       ][0]="COMM";
    m_frames[Composer       ][0]="TCOM";
    m_frames[Description    ][0]="TIT3";
    m_frames[Genre          ][0]="TCON";
    m_frames[Track          ][0]="TRCK";
    m_frames[Disc           ][0]="TPOS";
    m_frames[Rating         ][0]="POPM";
    m_frames[Year           ][0]="TYER";

    m_frames[Name           ][1]="TT2";
    m_frames[Artist         ][1]="TP1";
    m_frames[Album          ][1]="TAL";
    m_frames[AlbumArtist    ][1]="TP2";
    m_frames[BeatsPerMinuate][1]="TBP";
    m_frames[Category       ][1]="TT1";
    m_frames[Comments       ][1]="COM";
    m_frames[Composer       ][1]="TCM";
    m_frames[Description    ][1]="TT3";
    m_frames[Genre          ][1]="TCO";
    m_frames[Track          ][1]="TRK";
    m_frames[Disc           ][1]="TPA";
    m_frames[Rating         ][1]="POP";
    m_frames[Year           ][1]="TYE";
}

QString KNMusicTagID3v2::id3v2DataToString(const QByteArray &value) const
{
    QByteArray content=value;
    quint8 encoding=(quint8)(content.at(0));
    content.remove(0,1);
    int lastCharIndex=content.size()-1;
    switch(encoding)
    {
    case 0:
        //ISO-8859-1
        if(lastCharIndex>-1 && content.at(lastCharIndex)==0)
        {
            content.remove(lastCharIndex, 1);
        }
        return m_isoCodec->toUnicode(content).simplified();
    case 1:
        //UTF-16 LE/BE
        if(lastCharIndex>0 && content.at(lastCharIndex)==0 && content.at(lastCharIndex-1)==0)
        {
            content.remove(lastCharIndex-1, 2);
        }
        if((quint8)content.at(0)==0xFE && (quint8)content.at(1)==0xFF)
        {
            return m_beCodec->toUnicode(content).simplified();
        }
        if((quint8)content.at(0)==0xFF && (quint8)content.at(1)==0xFE)
        {
            return m_leCodec->toUnicode(content).simplified();
        }
        return m_utf16Codec->toUnicode(content).simplified();
    case 2:
        if(lastCharIndex>0 && content.at(lastCharIndex)==0 && content.at(lastCharIndex-1)==0)
        {
            content.remove(lastCharIndex-1, 2);
        }
        //UTF-16 BE without BOM
        return m_beCodec->toUnicode(content).simplified();
    case 3:
        //UTF-8
        if(lastCharIndex>-1 && content.at(lastCharIndex)==0)
        {
            content.remove(lastCharIndex, 1);
        }
        return m_utf8Codec->toUnicode(content).simplified();
    default:
        return m_localCodec->toUnicode(content).simplified();
    }
}

int KNMusicTagID3v2::hexToStarRating(const quint8 &hex) const
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


QString KNMusicTagID3v2::textData(const int &key) const
{
    int frameDataIndex=m_frameID.indexOf(m_frames[key][m_useShortFrames]);
    if(frameDataIndex==-1)
    {
        return QString();
    }
    return id3v2DataToString(m_frameData.at(frameDataIndex));
}

int KNMusicTagID3v2::id3v2RatingData() const
{
    int frameDataIndex=m_frameID.indexOf(m_frames[Rating][m_useShortFrames]);
    if(frameDataIndex==-1)
    {
        return 0;
    }
    QByteArray ratingRaw=m_frameData.at(frameDataIndex);
    QString windowsMediaTest=QString(ratingRaw);
    if(windowsMediaTest.indexOf("Windows Media Player")!=-1)
    {
        return hexToStarRating((quint8)ratingRaw.at(ratingRaw.size()-1));
    }
    return 0;
}

void KNMusicTagID3v2::clearCache()
{
    m_version=0;
    m_revision=0;
    m_unsynchronisation=false;
    m_extendedHeader=false;
    m_experimentalIndicator=false;
    m_frameID.clear();
    m_frameData.clear();
    m_tagImages.clear();
}

bool KNMusicTagID3v2::readTag(const QFile &mediaFile,
                              QDataStream &mediaData)
{
    clearCache();
    if(mediaFile.size()<10)
    {
        //If file is less than ID3v2 header, it can't contains ID3v2 tag.
        return false;
    }
    //Detect ID3v2 header.
    mediaData.readRawData(m_header, 10);
    if(!parseHeaderData())
    {
        //Parse header error.
        return false;
    }
    if(mediaFile.size()<((qint64)m_tagSize+10))
    {
        //File is smaller than the tag says, failed to get.
        return false;
    }

    m_rawTagData=new char[m_tagSize];
    mediaData.readRawData(m_rawTagData, m_tagSize);
    parseRawData();
    //All process code above.
    delete[] m_rawTagData; //Don't touch this.
    if(!m_frameID.isEmpty())
    {
        m_useShortFrames=(m_frameID.first().length()==3);
    }
    return true;
}

bool KNMusicTagID3v2::parseHeaderData()
{
    if(m_header[0]!='I' || m_header[1]!='D' || m_header[2]!='3')
    {
        //Can't find 'ID3' from the very beginning.
        return false;
    }
    m_tagSize=(((quint32)m_header[6]<<21)&0b00001111111000000000000000000000)+
              (((quint32)m_header[7]<<14)&0b00000000000111111100000000000000)+
              (((quint32)m_header[8]<<7) &0b00000000000000000011111110000000)+
              ( (quint32)m_header[9]     &0b00000000000000000000000001111111);
    m_version=(int)m_header[3];
    m_revision=(int)m_header[4];
    m_id3v23Later=(m_version>2);
    //Process header: header[5]
    m_unsynchronisation    =(m_header[5]&0b10000000);
    m_extendedHeader       =(m_header[5]&0b01000000);
    m_experimentalIndicator=(m_header[5]&0b00100000);
    return true;
}

void KNMusicTagID3v2::parseRawData()
{
    //All process code here.
    quint32 rawPosition=0, frameSize;
    char rawFrameID[5];
    if(m_id3v23Later)
    {
        rawFrameID[4]='\0';
        while(rawPosition<m_tagSize)
        {
            strncpy(rawFrameID, m_rawTagData+rawPosition, 4);
            if(rawFrameID[0]==0)
            {
                //If no tags, means behind of these datas are all '\0'.
                break;
            }
            frameSize=m_version==3?
                      (((quint32)m_rawTagData[rawPosition+4]<<24)&0b11111111000000000000000000000000)+
                      (((quint32)m_rawTagData[rawPosition+5]<<16)&0b00000000111111110000000000000000)+
                      (((quint32)m_rawTagData[rawPosition+6]<<8 )&0b00000000000000001111111100000000)+
                      ( (quint32)m_rawTagData[rawPosition+7]     &0b00000000000000000000000011111111):
                      (((quint32)m_rawTagData[rawPosition+4]<<21)&0b00001111111000000000000000000000)+
                      (((quint32)m_rawTagData[rawPosition+5]<<14)&0b00000000000111111100000000000000)+
                      (((quint32)m_rawTagData[rawPosition+6]<<7 )&0b00000000000000000011111110000000)+
                      ( (quint32)m_rawTagData[rawPosition+7]     &0b00000000000000000000000001111111);
            if(frameSize>m_tagSize)
            {
                //Reach an unexpect frame.
                break;
            }
            rawPosition+=10;
            QByteArray frameData=QByteArray(m_rawTagData+rawPosition, frameSize);
            m_frameID.append(rawFrameID);
            m_frameData.append(frameData);
            if(QString(rawFrameID)=="PIC")
            {
                processPIC(frameData);
            }
            if(QString(rawFrameID)=="APIC")
            {
                processAPIC(frameData);
            }
            rawPosition+=frameSize;
        }
    }
    else
    {
        rawFrameID[3]='\0';
        while(rawPosition<m_tagSize)
        {
            strncpy(rawFrameID, m_rawTagData+rawPosition, 3);
            if(rawFrameID[0]==0)
            {
                //If no tags, means behind of these datas are all '\0'.
                break;
            }
            frameSize=(((quint32)m_rawTagData[rawPosition+3]<<16)&0b00000000111111110000000000000000)+
                      (((quint32)m_rawTagData[rawPosition+4]<<8) &0b00000000000000001111111100000000)+
                      ( (quint32)m_rawTagData[rawPosition+5]     &0b00000000000000000000000011111111);
            if(frameSize>m_tagSize)
            {
                //Reach an unexpect frame.
                break;
            }
            rawPosition+=6;
            QByteArray frameData=QByteArray(m_rawTagData+rawPosition, frameSize);
            m_frameID.append(rawFrameID);
            m_frameData.append(frameData);
            if(QString(rawFrameID)=="PIC")
            {
                processPIC(frameData);
            }
            rawPosition+=frameSize;
        }
    }
}

void KNMusicTagID3v2::processAPIC(const QByteArray &value)
{
    ID3v2Image currentImage;
    QByteArray content=value;
    quint8 encoding=(quint8)(value.at(0));
    int zeroCharEnd=content.indexOf('\0', 1);
    QString mimeType(content.mid(1, zeroCharEnd-1).toLower());
    if(mimeType.length()>6)
    {
        mimeType.remove(0, 6);
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
            QString::fromLocal8Bit(content.mid(zeroCharEnd, descriptionEnd-zeroCharEnd+1));
        content.remove(0, descriptionEnd+1);
        break;
    case 1:
        //UTF-16
        descriptionEnd=zeroCharEnd;
        while(content.at(descriptionEnd)!=0 &&
              content.at(descriptionEnd+1)!=0)
        {
            descriptionEnd+=2;
        }
        if((quint8)content.at(zeroCharEnd)==0xFE && (quint8)content.at(zeroCharEnd+1)==0xFF)
        {
            content.remove(0,2);
            currentImage.description=m_beCodec->toUnicode(content.mid(zeroCharEnd,
                                                                      descriptionEnd-zeroCharEnd+1));
        }
        if((quint8)content.at(zeroCharEnd)==0xFF && (quint8)content.at(zeroCharEnd+1)==0xFE)
        {
            content.remove(0,2);
            currentImage.description=m_leCodec->toUnicode(content.mid(zeroCharEnd,
                                                                      descriptionEnd-zeroCharEnd+1));
        }
        content.remove(0, descriptionEnd+2);
        break;
    default:
        content.remove(0, content.indexOf('\0', zeroCharEnd)+1);
        break;
    }
    currentImage.image.loadFromData(content);
    m_tagImages[pictureType]=currentImage;
}

void KNMusicTagID3v2::processPIC(const QByteArray &value)
{
    ID3v2Image currentImage;
    QByteArray content=value;
    quint8 encoding=(quint8)(value.at(0));
    QString imageType(content.mid(1, 3).toLower());
    int descriptionEnd;
    content.remove(0, 5);
    switch(encoding)
    {
    case 0:
        //ISO
        descriptionEnd=content.indexOf('\0');
        currentImage.description=
            QString::fromLocal8Bit(content.left(descriptionEnd));
        content.remove(0, descriptionEnd+1);
        break;
    case 1:
        //UTF-16
        descriptionEnd=0;
        while(content.at(descriptionEnd)!=0 &&
              content.at(descriptionEnd+1)!=0)
        {
            descriptionEnd+=2;
        }
        if((quint8)content.at(5)==0xFE && (quint8)content.at(6)==0xFF)
        {
            content.remove(0,2);
            currentImage.description=m_beCodec->toUnicode(content.left(descriptionEnd-4));
        }
        if((quint8)content.at(5)==0xFF && (quint8)content.at(6)==0xFE)
        {
            content.remove(0,2);
            currentImage.description=m_leCodec->toUnicode(content.left(descriptionEnd-4));
        }
        content.remove(0, descriptionEnd+1);
        break;
    default:
        content.remove(0, content.indexOf('\0')+1);
        break;
    }
    //Tweak;
    while(!content.isEmpty() && content.at(0)=='\0')
    {
        content.remove(0,1);
    }
    currentImage.image.loadFromData(content);
    m_tagImages[(quint8)value.at(4)]=currentImage;
}

QImage KNMusicTagID3v2::tagImage(const int &index) const
{
    return m_tagImages[index].image;
}

QImage KNMusicTagID3v2::firstAvaliableImage() const
{
    if(m_tagImages.size()!=0)
    {
        return m_tagImages.first().image;
    }
    return QImage();
}

void KNMusicTagID3v2::setHeaderData(char *rawData)
{
    memcpy(m_header, rawData, 10);
}

void KNMusicTagID3v2::setRawData(char *rawData, const quint32 &rawDataLength)
{
    m_rawTagData=rawData;
    m_tagSize=rawDataLength;
}
