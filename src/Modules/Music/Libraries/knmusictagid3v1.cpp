#include <QDataStream>
#include <QFile>
#include <QTextCodec>

#include <QDebug>

#include <cstdlib>

#include "../knmusicglobal.h"

#include "knmusictagid3v1.h"

KNMusicTagID3v1::KNMusicTagID3v1(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_codec=QTextCodec::codecForName("GBK");
}

bool KNMusicTagID3v1::readTag(const QString &filePath)
{
    clearCache();
    //Get raw tag data
    QFile mediaFile(filePath);
    if(mediaFile.size()<128)
    {
        //If the size is less than 128, it can't contains ID3v1.
        return false;
    }
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);
    mediaData.skipRawData(mediaFile.size()-128);
    char rawTagData[128];
    mediaData.readRawData(rawTagData, 128);
    mediaFile.close();

    if(rawTagData[0]!='T' || rawTagData[1]!='A' || rawTagData[2]!='G')
    {
        //Can't find 'TAG'.
        return false;
    }
    char rawText[30];
    strncpy(rawText, rawTagData+3, 30);
    m_tagData[Title]=QString::fromLocal8Bit(rawText).simplified();
    strncpy(rawText, rawTagData+33, 30);
    m_tagData[Artist]=QString::fromLocal8Bit(rawText).simplified();
    strncpy(rawText, rawTagData+63, 30);
    m_tagData[Album]=QString::fromLocal8Bit(rawText).simplified();
    strncpy(rawText, rawTagData+93, 4);
    rawText[4]='\0';
    m_tagData[Year]=QString::fromLocal8Bit(rawText).simplified();
    if(rawTagData[125]==0)
    {
        m_tagData[Track]=QString::number((int)rawTagData[126]);
        strncpy(rawText, rawTagData+97, 28);
        m_tagData[Comment]=QString::fromLocal8Bit(rawText).simplified();
    }
    else
    {
        m_tagData[Track]="0";
        strncpy(rawText, rawTagData+97, 30);
        m_tagData[Comment]=QString::fromLocal8Bit(rawText).simplified();
    }
    m_tagData[Genre]=KNMusicGlobal::instance()->getGenre((int)rawTagData[127]);
    return true;
}

void KNMusicTagID3v1::clearCache()
{
    m_tagData[Title].clear();
    m_tagData[Artist].clear();
    m_tagData[Album].clear();
    m_tagData[Year].clear();
    m_tagData[Comment].clear();
    m_tagData[Track]="0";
    m_tagData[Genre].clear();
}

QString KNMusicTagID3v1::textData(const int &key) const
{
    return m_tagData[key];
}
