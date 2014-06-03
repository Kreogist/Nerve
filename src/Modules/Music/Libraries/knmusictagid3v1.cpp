#include <QTextCodec>

#include <QDebug>

#include <cstdlib>

#include "../../knglobal.h"
#include "../knmusicglobal.h"

#include "knmusictagid3v1.h"

KNMusicTagID3v1::KNMusicTagID3v1(QObject *parent) :
    KNMusicTagBase(parent)
{
    //Set the empty tag sample.
    memset(m_emptyTag, 0, 128);
    m_emptyTag[0]='T';m_emptyTag[1]='A';m_emptyTag[2]='G';
    m_emptyTag[127]=0xff;
    //Initial the locale codec.
    m_localCodec=KNGlobal::instance()->codecForCurrentLocale();
}

bool KNMusicTagID3v1::readTag(const QFile &mediaFile,
                              QDataStream &mediaData)
{
    //Get raw tag data
    int mediaFileSize=mediaFile.size();
    if(mediaFileSize<128)
    {
        clearCache();
        //If the size is less than 128, it can't contains ID3v1.
        return false;
    }
    //Read the last 128 byte data.
    mediaData.skipRawData(mediaFileSize-128);
    mediaData.readRawData(m_rawTagData, 128);

    //Check the header, if header is not 'TAG', return false.
    if(m_rawTagData[0]!='T' || m_rawTagData[1]!='A' || m_rawTagData[2]!='G')
    {
        return false;
    }

    //Read the tag data.
    //ID3v1's data is stored in order:
    //30 bytes Title, 30 bytes Artist, 30 bytes Album, 4 bytes Year,
    //30 bytes Comments(+Track), 1 bytes Genre order. Just read them
    m_rawByteBackup=m_rawTagData[33];
    m_rawTagData[33]=0;
    m_tagData[Title]=m_localCodec->toUnicode(m_rawTagData+3).simplified();
    m_rawTagData[33]=m_rawByteBackup;
    m_rawByteBackup=m_rawTagData[63];
    m_rawTagData[63]=0;
    m_tagData[Artist]=m_localCodec->toUnicode(m_rawTagData+33).simplified();
    m_rawTagData[63]=m_rawByteBackup;
    m_rawByteBackup=m_rawTagData[93];
    m_rawTagData[93]=0;
    m_tagData[Album]=m_localCodec->toUnicode(m_rawTagData+63).simplified();
    m_rawTagData[93]=m_rawByteBackup;
    m_rawByteBackup=m_rawTagData[97];
    m_rawTagData[97]=0;
    m_tagData[Year]=m_localCodec->toUnicode(m_rawTagData+93).simplified();
    m_rawTagData[97]=m_rawByteBackup;
    //If the No.125's char of tag is 0, then it means the following char is
    //Track Number.
    if(m_rawTagData[125]==0)
    {
        m_tagData[Track]=QString::number((quint8)m_rawTagData[126]);
        m_tagData[Comment]=m_localCodec->toUnicode(m_rawTagData+97).simplified();
    }
    else
    {
        m_rawByteBackup=m_rawTagData[127];
        m_rawTagData[127]=0;
        m_tagData[Comment]=m_localCodec->toUnicode(m_rawTagData+97, 30).simplified();
        m_rawTagData[127]=m_rawByteBackup;
    }
    m_genreIndex=(quint8)m_rawTagData[127];
    m_tagData[Genre]=KNMusicGlobal::instance()->getGenre(m_genreIndex);
    return true;
}

void KNMusicTagID3v1::clearCache()
{
    //Clear the QStrings.
    for(int i=0; i<ID3v1ItemCount; i++)
    {
        m_tagData[i].clear();
    }
}

QString KNMusicTagID3v1::textData(const int &key) const
{
    return m_tagData[key];
}

void KNMusicTagID3v1::setTextData(const int &key, const QString &data)
{
    m_tagData[key]=data;
}

void KNMusicTagID3v1::writeTag(QFile &mediaFile, QDataStream &mediaData)
{
    //Clear the tag.
    memset(m_rawTagData, 0, 128);
    //Set the ID3v1's header.
    m_rawTagData[0]='T';
    m_rawTagData[1]='A';
    m_rawTagData[2]='G';
    //Copy all the values of the ID3v1 to the file.
    strncpy(m_rawTagData+3, m_localCodec->fromUnicode(m_tagData[Title]).data(), 30);
    strncpy(m_rawTagData+33, m_localCodec->fromUnicode(m_tagData[Artist]).data(), 30);
    strncpy(m_rawTagData+63, m_localCodec->fromUnicode(m_tagData[Album]).data(), 30);
    strncpy(m_rawTagData+93, m_localCodec->fromUnicode(m_tagData[Year]).data(), 4);
    int trackTest=m_tagData[Track].toInt();
    if(trackTest>0 && trackTest<255)
    {
        m_rawTagData[125]=0;
        m_rawTagData[126]=trackTest;
        strncpy(m_rawTagData+97, m_localCodec->fromUnicode(m_tagData[Comment]).data(), 28);
    }
    else
    {
        strncpy(m_rawTagData+97, m_localCodec->fromUnicode(m_tagData[Comment]).data(), 30);
    }
    //Check the genre index to ensure the index is in the range of 0x00-0xff.
    int genreTest=KNMusicGlobal::instance()->genreIndex(m_tagData[Genre]);
    if(genreTest==-1)
    {
        m_rawTagData[127]=0xff;
    }
    else
    {
        m_rawTagData[127]=genreTest;
    }
    //Check if the new tag is equal to the empty ID3v1 tag:
    //It means: No values, no tracks, genre is invaild. We will remove ID3v1 tags.
    bool isTagEmpty=(memcmp(m_rawTagData, m_emptyTag, 127)==0);
    int mediaFileSize=mediaFile.size();
    if(mediaFileSize>128)
    {
        //If the file size is greater than 128, it might contains ID3v1.
        mediaData.skipRawData(mediaFileSize-128);
        //Read the header
        char header[3];
        mediaData.readRawData(header, 3);
        //If header is not 'TAG', then set the position to the end of file.
        if(header[0]!='T' || header[1]!='A' || header[2]!='G')
        {
            if(isTagEmpty)
            {
                return;
            }
            mediaData.skipRawData(125);
        }
        else
        {
            //Find ID3v1 file.
            //If new tag is empty, that means we need to remove the original tag.
            if(isTagEmpty)
            {
                mediaFile.resize(mediaFileSize-128);
                return;
            }
            mediaFile.reset();
            mediaData.skipRawData(mediaFileSize-128);
        }
    }
    else
    {
        if(isTagEmpty)
        {
            return;
        }
        mediaData.skipRawData(mediaFileSize);
    }
    mediaData.writeRawData(m_rawTagData, 128);
}

quint8 KNMusicTagID3v1::genreIndex() const
{
    return m_genreIndex;
}
