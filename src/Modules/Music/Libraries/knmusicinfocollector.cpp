
#include <QDebug>

#include <QAudioBuffer>
#include <QDateTime>
#include <QFileInfo>

#include "../knmusicglobal.h"
#include "knmusictagid3v1.h"
#include "knmusictagid3v2.h"
#include "knmusictagapev2.h"
#include "knmusictagwma.h"

#include "../../knglobal.h"

#include "knmusicinfocollector.h"

KNMusicInfoCollector::KNMusicInfoCollector(QObject *parent) :
    QObject(parent)
{
    m_global=KNGlobal::instance();
    m_musicGlobal=KNMusicGlobal::instance();
    m_tagID3v1=new KNMusicTagID3v1(this);
    m_tagID3v2=new KNMusicTagID3v2(this);
    m_tagAPEv2=new KNMusicTagAPEv2(this);
    m_tagWMA=new KNMusicTagWma(this);
}

void KNMusicInfoCollector::analysis(const QString &filePath)
{   
    resetMusicInfo();
    KNMusicGlobal::MusicDetailsInfo currentFileInfo;
    QFileInfo currentFile(filePath);
    m_musicInfos[KNMusicGlobal::Name]=currentFile.fileName();

    m_musicInfos[KNMusicGlobal::Size]=
            m_global->byteToHigher(currentFile.size());
    currentFileInfo.size=currentFile.size();
    m_musicInfos[KNMusicGlobal::DateModified]=
            currentFile.lastModified().toString("yyyy-MM-dd APhh:mm");
    currentFileInfo.dateModified=currentFile.lastModified();
    m_musicInfos[KNMusicGlobal::LastPlayed]=
            currentFile.lastRead().toString("yyyy-MM-dd APhh:mm");
    currentFileInfo.lastPlayed=currentFile.lastRead();
    QDateTime addedDate=QDateTime::currentDateTime();
    m_musicInfos[KNMusicGlobal::DateAdded]=
            addedDate.toString("yyyy-MM-dd APhh:mm");
    currentFileInfo.dateAdded=addedDate;

    readID3v1Tag(filePath);
    readID3v2Tag(filePath);
    readAPEv2Tag(filePath);
    readWMATag(filePath);
    currentFileInfo.coverImage=m_musicCover;

    QStringList musicInfo;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        musicInfo.append(m_musicInfos[i]);
    }

    emit requireAppendMusic(musicInfo,
                            currentFileInfo);
}

void KNMusicInfoCollector::resetMusicInfo()
{
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        m_musicInfos[i].clear();
    }
    m_musicCover=QPixmap();
}

void KNMusicInfoCollector::readID3v1Tag(const QString &value)
{
    if(m_tagID3v1->readTag(value))
    {
        KNMusicTagID3v1::ID3v1Data id3v1Info=m_tagID3v1->tagData();
        setMediaData(KNMusicGlobal::Name,id3v1Info.title);
        setMediaData(KNMusicGlobal::Artist,id3v1Info.artist);
        setMediaData(KNMusicGlobal::Album,id3v1Info.album);
        setMediaData(KNMusicGlobal::Genre,id3v1Info.genre);
        setMediaData(KNMusicGlobal::Year,id3v1Info.year);
        setMediaData(KNMusicGlobal::Comments,id3v1Info.comment);
        setMediaData(KNMusicGlobal::TrackNumber,QString::number((quint8)id3v1Info.track));
    }
}

void KNMusicInfoCollector::readID3v2Tag(const QString &value)
{
    if(m_tagID3v2->readTag(value))
    {
        if(m_tagID3v2->version()>2)
        {
            setMediaData(KNMusicGlobal::Name,m_tagID3v2->id3v2String("TIT2"));
            setMediaData(KNMusicGlobal::Artist,m_tagID3v2->id3v2String("TPE1"));
            setMediaData(KNMusicGlobal::Album,m_tagID3v2->id3v2String("TALB"));
            setMediaData(KNMusicGlobal::AlbumArtist,m_tagID3v2->id3v2String("TPE2"));
            setMediaData(KNMusicGlobal::BeatsPerMinuate,m_tagID3v2->id3v2String("TBPM"));
            setMediaData(KNMusicGlobal::Category,m_tagID3v2->id3v2String("TIT1"));
            setMediaData(KNMusicGlobal::Composer,m_tagID3v2->id3v2String("TCOM"));
            setMediaData(KNMusicGlobal::Genre,
                         m_musicGlobal->getGenre(m_tagID3v2->id3v2String("TCON")));
            setMediaData(KNMusicGlobal::Year,m_tagID3v2->id3v2String("TYER"));
            QString trackInfo=m_tagID3v2->id3v2String("TRCK");
            int diagonalPos=trackInfo.indexOf("/");
            if(diagonalPos!=-1)
            {
                trackInfo=trackInfo.left(diagonalPos);
            }
            setMediaData(KNMusicGlobal::TrackNumber,trackInfo);
        }
        else
        {
            setMediaData(KNMusicGlobal::Name,m_tagID3v2->id3v2String("TT2"));
            setMediaData(KNMusicGlobal::Artist,m_tagID3v2->id3v2String("TP1"));
            setMediaData(KNMusicGlobal::Album,m_tagID3v2->id3v2String("TAL"));
            setMediaData(KNMusicGlobal::AlbumArtist,m_tagID3v2->id3v2String("TP2"));
            setMediaData(KNMusicGlobal::BeatsPerMinuate,m_tagID3v2->id3v2String("TBP"));
            setMediaData(KNMusicGlobal::Category,m_tagID3v2->id3v2String("TT1"));
            setMediaData(KNMusicGlobal::Composer,m_tagID3v2->id3v2String("TCM"));
            setMediaData(KNMusicGlobal::Genre,
                         m_musicGlobal->getGenre(m_tagID3v2->id3v2String("TCO")));
            setMediaData(KNMusicGlobal::Year,m_tagID3v2->id3v2String("TYE"));
        }
        m_musicCover=m_tagID3v2->tagImage(3); //3 is the Cover front.
        if(m_musicCover.isNull())
        {
            m_musicCover=m_tagID3v2->firstAvaliableImage();
        }
    }
}

void KNMusicInfoCollector::readAPEv2Tag(const QString &value)
{
    if(m_tagAPEv2->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name,m_tagAPEv2->tagStringData("Title"));
        setMediaData(KNMusicGlobal::Artist,m_tagAPEv2->tagStringData("Artist"));
        setMediaData(KNMusicGlobal::Album,m_tagAPEv2->tagStringData("Album"));
        //setMediaData(KNMusicGlobal::AlbumArtist,m_tagAPEv2->tagData("TP2"));
        //setMediaData(KNMusicGlobal::BeatsPerMinuate,m_tagID3v2->id3v2String("TBP"));
        //setMediaData(KNMusicGlobal::Category,m_tagID3v2->id3v2String("TT1"));
        setMediaData(KNMusicGlobal::Composer,m_tagAPEv2->tagStringData("Composer"));
        setMediaData(KNMusicGlobal::Genre,m_tagAPEv2->tagStringData("Genre"));
        //setMediaData(KNMusicGlobal::Year,m_tagID3v2->id3v2String("TYE"));
    }
}

void KNMusicInfoCollector::readWMATag(const QString &value)
{
    //m_tagWMA->readTag(value);
}

void KNMusicInfoCollector::setMediaData(const int &index,
                                        const QString &value)
{
    if(!value.isEmpty())
    {
        m_musicInfos[index]=value;
    }
}
