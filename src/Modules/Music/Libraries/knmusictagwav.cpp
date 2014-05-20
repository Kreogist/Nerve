#include <QMap>

#include <QDebug>

#include "knmusictagwav.h"

KNMusicTagWAV::KNMusicTagWAV(QObject *parent) :
    KNMusicTagBase(parent)
{
}

bool KNMusicTagWAV::readTag(const QFile &mediaFile,
                            QDataStream &mediaData)
{
    if(mediaFile.size()<12)
    {
        return false;
    }
    mediaData.readRawData(m_wavHeader, 12);
    //Check header;
    if(m_wavHeader[0]!='R' || m_wavHeader[1]!='I'  || m_wavHeader[2]!='F'  || m_wavHeader[3]!='F' ||
            m_wavHeader[8]!='W' || m_wavHeader[9]!='A'  || m_wavHeader[10]!='V'  || m_wavHeader[11]!='E')
    {
        return false;
    }
    quint32 chunkSize, fileSize=mediaFile.size();
    m_findList=false;m_findID3=false;
    while(mediaData.device()->pos()<fileSize && (m_findList==false || m_findID3==false))
    {
        mediaData.readRawData(m_wavChunkHeader, 8);
        chunkSize=(((quint32)m_wavChunkHeader[7]<<24)&0b11111111000000000000000000000000)+
                  (((quint32)m_wavChunkHeader[6]<<16)&0b00000000111111110000000000000000)+
                  (((quint32)m_wavChunkHeader[5]<<8 )&0b00000000000000001111111100000000)+
                  ( (quint32)m_wavChunkHeader[4]     &0b00000000000000000000000011111111);
        if(m_wavChunkHeader[0]=='L' && m_wavChunkHeader[1]=='I'
                && m_wavChunkHeader[2]=='S' && m_wavChunkHeader[3]=='T')
        {
            m_rawChunkData=new char[chunkSize];
            mediaData.readRawData(m_rawChunkData, chunkSize);
            m_chunkSize=chunkSize;
            parseLIST();
            delete[] m_rawChunkData;
            m_findList=true;
            continue;
        }
        if(m_wavChunkHeader[0]=='i' && m_wavChunkHeader[1]=='d'
                && m_wavChunkHeader[2]=='3' && m_wavChunkHeader[3]==0x20)
        {
            m_rawChunkData=new char[chunkSize];
            mediaData.readRawData(m_rawChunkData, chunkSize);
            m_chunkSize=chunkSize;
            parseID32();
            delete[] m_rawChunkData;
            m_findID3=true;
            continue;
        }
        mediaData.skipRawData(chunkSize);
    }
    return true;
}

QString KNMusicTagWAV::textData(const int &key) const
{
    return m_wavFrameData[key];
}

void KNMusicTagWAV::parseLIST()
{
    if(m_rawChunkData[0]!='I' || m_rawChunkData[1]!='N' ||
            m_rawChunkData[2]!='F' || m_rawChunkData[3]!='O')
    {
        return;
    }
    QMap<QString, QString> listData;
    quint32 position=4, frameSize;
    while(position<m_chunkSize)
    {
        frameSize=(((quint32)m_rawChunkData[position+7]<<24)&0b11111111000000000000000000000000)+
                  (((quint32)m_rawChunkData[position+6]<<16)&0b00000000111111110000000000000000)+
                  (((quint32)m_rawChunkData[position+5]<<8 )&0b00000000000000001111111100000000)+
                  ( (quint32)m_rawChunkData[position+4]     &0b00000000000000000000000011111111);
        listData[QByteArray(m_rawChunkData+position, 4)]=QString(QByteArray(m_rawChunkData+position+8, frameSize));
        position+=frameSize+8;
        while(m_rawChunkData[position]==0)
        {
            position++;
        }
    }

    m_wavFrameData[Artist]=listData["IART"];
    m_wavFrameData[Album]=listData["IPRD"];
    m_wavFrameData[BeatsPerMinuate]=listData["IBPM"];
    m_wavFrameData[Comments]=listData["ICMT"];
    m_wavFrameData[Genre]=listData["IGNR"];
    m_wavFrameData[Name]=listData["INAM"];
    m_wavFrameData[Track]=listData["ITRK"];
    m_wavFrameData[Year]=listData["ICRD"];
}

void KNMusicTagWAV::parseID32()
{
    qDebug()<<"ID32";
    qDebug()<<m_chunkSize;
}
