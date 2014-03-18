
#include <QFileInfo>

#include "knmusictagid3v1.h"
#include "knmusictagid3v2.h"

#include "knmusicinfocollector.h"

KNMusicInfoCollector::KNMusicInfoCollector(QObject *parent) :
    QObject(parent)
{
    m_tagID3v1=new KNMusicTagID3v1(this);
    m_tagID3v2=new KNMusicTagID3v2(this);
}

void KNMusicInfoCollector::analysis(const QString &filePath)
{
    resetMusicInfo();
    QFileInfo currentFile(filePath);
    m_musicInfos[KNMusicGlobal::Name]=currentFile.fileName();

    if(m_tagID3v1->readTag(filePath))
    {
        KNMusicTagID3v1::ID3v1Data id3v1Info=m_tagID3v1->tagData();
        if(!id3v1Info.title.isEmpty())
        {
            m_musicInfos[KNMusicGlobal::Name]=id3v1Info.title;
        }
        m_musicInfos[KNMusicGlobal::Artist]=id3v1Info.artist;
        m_musicInfos[KNMusicGlobal::Album]=id3v1Info.album;
        m_musicInfos[KNMusicGlobal::Genre]=id3v1Info.genre;
        m_musicInfos[KNMusicGlobal::Year]=id3v1Info.year;
    }

    if(m_tagID3v2->readTag(filePath))
    {
        QString title=m_musicInfos[KNMusicGlobal::Album]=m_tagID3v2->id3v2String("TIT2");
        if(!title.isEmpty())
        {
            m_musicInfos[KNMusicGlobal::Name]=title;
        }
        m_musicInfos[KNMusicGlobal::Artist]=m_tagID3v2->id3v2String("TPE1");
        m_musicInfos[KNMusicGlobal::Album]=m_tagID3v2->id3v2String("TALB");
        m_musicInfos[KNMusicGlobal::BeatsPerMinuate]=m_tagID3v2->id3v2String("TBPM");
        m_musicInfos[KNMusicGlobal::Composer]=m_tagID3v2->id3v2String("TCOM");
    }

    QStringList musicInfo;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        musicInfo.append(m_musicInfos[i]);
    }
    emit requireAppendMusic(musicInfo);
}

void KNMusicInfoCollector::resetMusicInfo()
{
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        m_musicInfos[i].clear();
    }
}
