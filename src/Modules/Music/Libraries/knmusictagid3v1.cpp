#include <QTextCodec>

#include <QDebug>

#include <cstdlib>

#include "../../knglobal.h"
#include "../knmusicglobal.h"

#include "knmusictagid3v1.h"

KNMusicTagID3v1::KNMusicTagID3v1(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_localCodec=KNGlobal::instance()->codecForCurrentLocale();
}

bool KNMusicTagID3v1::readTag(const QFile &mediaFile,
                              QDataStream &mediaData)
{
    clearCache();
    //Get raw tag data
    int mediaFileSize=mediaFile.size();
    if(mediaFileSize<128)
    {
        //If the size is less than 128, it can't contains ID3v1.
        return false;
    }
    mediaData.skipRawData(mediaFileSize-128);
    mediaData.readRawData(m_rawTagData, 128);

    if(m_rawTagData[0]!='T' || m_rawTagData[1]!='A' || m_rawTagData[2]!='G')
    {
        return false;
    }
    strncpy(m_rawText, m_rawTagData+3, 30);
    m_tagData[Title]=m_localCodec->toUnicode(m_rawText).simplified();
    strncpy(m_rawText, m_rawTagData+33, 30);
    m_tagData[Artist]=m_localCodec->toUnicode(m_rawText).simplified();
    strncpy(m_rawText, m_rawTagData+63, 30);
    m_tagData[Album]=m_localCodec->toUnicode(m_rawText).simplified();
    strncpy(m_rawText, m_rawTagData+93, 4);
    m_rawText[4]='\0';
    m_tagData[Year]=m_localCodec->toUnicode(m_rawText).simplified();
    if(m_rawTagData[125]==0)
    {
        m_tagData[Track]=QString::number((int)m_rawTagData[126]);
        strncpy(m_rawText, m_rawTagData+97, 28);
        m_tagData[Comment]=m_localCodec->toUnicode(m_rawText).simplified();
    }
    else
    {
        strncpy(m_rawText, m_rawTagData+97, 30);
        m_tagData[Comment]=m_localCodec->toUnicode(m_rawText).simplified();
    }
    m_tagData[Genre]=KNMusicGlobal::instance()->getGenre((int)m_rawTagData[127]);
    return true;
}

void KNMusicTagID3v1::clearCache()
{
    m_tagData[Title].clear();
    m_tagData[Artist].clear();
    m_tagData[Album].clear();
    m_tagData[Year].clear();
    m_tagData[Comment].clear();
    m_tagData[Track].clear();
    m_tagData[Genre].clear();
}

QString KNMusicTagID3v1::textData(const int &key) const
{
    return m_tagData[key];
}
