#include <QFile>
#include <QDataStream>

#include <QDebug>

#include "knmusictagflac.h"

KNMusicTagFLAC::KNMusicTagFLAC(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_frames[Name       ]="title";
    m_frames[Artist     ]="artist";
    m_frames[Album      ]="album";
    m_frames[Genre      ]="genre";
    m_frames[Description]="description";
    m_frames[Composer   ]="composer";
    m_frames[DiscCount  ]="totaldiscs";
    m_frames[DiscNumber ]="discnumber";
    m_frames[Comments   ]="comment";
    m_frames[AlbumArtist]="albumartist";
    m_frames[TrackCount ]="tracktotal";
    m_frames[Year       ]="date";
    m_frames[TrackNumber]="tracknumber";
}

bool KNMusicTagFLAC::readTag(const QFile &mediaFile,
                             QDataStream &mediaData)
{
    clearCache();
    /*QFile mediaFile(filePath);
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);*/
    char rawHeader[5];
    rawHeader[4]='\0';
    mediaData.readRawData(rawHeader, 4);
    if(rawHeader[0]!=0x66 || rawHeader[1]!=0x4C ||
            rawHeader[2]!=0x61 || rawHeader[3]!=0x43)
    {
        return false;
    }
    bool isLastMeta=false;
    while(!isLastMeta)
    {
        mediaData.readRawData(rawHeader, 4);
        MetadataHeader header=analysisHeader(rawHeader);
        isLastMeta=header.isLast;
        char *rawTagData=new char[header.size];
        mediaData.readRawData(rawTagData, header.size);
        /*
            0 : STREAMINFO
            1 : PADDING
            2 : APPLICATION
            3 : SEEKTABLE
            4 : VORBIS_COMMENT
            5 : CUESHEET
            6 : PICTURE
            7-126 : reserved
            127 : No use
        */
        switch(header.type)
        {
        case 4:
            parseVorbisComment(rawTagData, header.size);
            break;
        case 6:
            parsePicture(rawTagData);
            break;
        default:
            break;
        }
        delete[] rawTagData;
    }
    return true;
}

void KNMusicTagFLAC::clearCache()
{
    m_metadata.clear();
    m_picture.clear();
}

QString KNMusicTagFLAC::textData(const int &key) const
{
    return metaData(m_frames[key]);
}

QString KNMusicTagFLAC::metaData(const QString &index) const
{
    return m_metadata[index].simplified();
}

QString KNMusicTagFLAC::rawMetaData(const QString &index)
{
    return m_metadata[index];
}

QImage KNMusicTagFLAC::tagImage(const int &index) const
{
    return m_picture[index];
}

QImage KNMusicTagFLAC::firstAvaliableImage() const
{
    if(m_picture.size()!=0)
    {
        return m_picture.first();
    }
    return QImage();
}

void KNMusicTagFLAC::parseVorbisComment(char *rawTagData, int length)
{
    quint32 stringLength=inverseCharToInt32(rawTagData),
            pointer=stringLength+8,
            uLength=length, equalPos;
    QString comment;
    while(pointer<uLength)
    {
        stringLength=inverseCharToInt32(rawTagData+pointer);
        char *itemData=new char[stringLength+1];
        memcpy(itemData, rawTagData+pointer+4, stringLength);
        itemData[stringLength]='\0';
        comment=QString(itemData);
        equalPos=comment.indexOf('=');
        m_metadata[comment.left(equalPos).toLower()]=comment.mid(equalPos+1);
        //Move pointer.
        stringLength+=4;
        pointer+=stringLength;
    }
}

void KNMusicTagFLAC::parsePicture(char *rawTagData)
{
    quint32 imageType=charToInt32(rawTagData),
            mimeLength=charToInt32(rawTagData+4),
            pointer=8;
    char *rawMimeData=new char[mimeLength+1];
    rawMimeData[mimeLength]='\0';
    memcpy(rawMimeData, rawTagData+pointer, mimeLength);
    QString mimeType=rawMimeData, imageFormat;
    if(mimeType.left(6)=="image/")
    {
        imageFormat=mimeType.mid(6);
    }
    else
    {
        imageFormat=mimeType;
    }
    delete[] rawMimeData;
    pointer+=mimeLength;
    quint32 describeLength=charToInt32(rawTagData+pointer);
    pointer+=(describeLength+20);
    quint32 pictureDataLength=charToInt32(rawTagData+pointer);
    char *pictureRawData=new char[pictureDataLength];
    pointer+=4;
    memcpy(pictureRawData, rawTagData+pointer, pictureDataLength);
    QByteArray pictureData;
    pictureData.append(pictureRawData, pictureDataLength);
    delete[] pictureRawData;
    QImage currentImage;
    currentImage.loadFromData(pictureData);
    m_picture[imageType]=currentImage;
}

quint32 KNMusicTagFLAC::inverseCharToInt32(char *rawTagData)
{
    return (((quint32)rawTagData[3]<<24) & 0b11111111000000000000000000000000) +
           (((quint32)rawTagData[2]<<16) & 0b00000000111111110000000000000000) +
           (((quint32)rawTagData[1]<<8)  & 0b00000000000000001111111100000000) +
           ( (quint32)rawTagData[0]      & 0b00000000000000000000000011111111);
}

quint32 KNMusicTagFLAC::charToInt32(char *rawTagData)
{
    return (((quint32)rawTagData[0]<<24) & 0b11111111000000000000000000000000) +
           (((quint32)rawTagData[1]<<16) & 0b00000000111111110000000000000000) +
           (((quint32)rawTagData[2]<<8)  & 0b00000000000000001111111100000000) +
           ( (quint32)rawTagData[3]      & 0b00000000000000000000000011111111);
}

KNMusicTagFLAC::MetadataHeader KNMusicTagFLAC::analysisHeader(char *rawHeader)
{
    MetadataHeader header;
    header.isLast=((quint8)rawHeader[0]>>7)==1;
    header.type=((quint8)rawHeader[0] & 0b01111111);
    header.size=(((quint32)rawHeader[1]<<16) & 0b00000000111111110000000000000000) +
                (((quint32)rawHeader[2]<<8)  & 0b00000000000000001111111100000000) +
                ( (quint32)rawHeader[3]      & 0b00000000000000000000000011111111);
    return header;
}
