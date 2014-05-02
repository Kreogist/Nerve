#include <QDebug>

#include <QDateTime>
#include <QFileInfo>
#include <QUrl>
#include <QMap>

#include "../knmusicglobal.h"
#include "../../Public/knlibmediainfo.h"
#include "knmusictagid3v1.h"
#include "knmusictagid3v2.h"
#include "knmusictagapev2.h"
#include "knmusictagwma.h"
#include "knmusictagm4a.h"

#include "../../knglobal.h"

#include "knmusicinfocollector.h"

KNMusicInfoCollector::KNMusicInfoCollector(QObject *parent) :
    KNLibInfoCollector(parent)
{
    m_global=KNGlobal::instance();
    m_musicGlobal=KNMusicGlobal::instance();
    m_mediaInfo=new KNLibMediaInfo(this);
    m_tagID3v1=new KNMusicTagID3v1(this);
    m_tagID3v2=new KNMusicTagID3v2(this);
    m_tagAPEv2=new KNMusicTagAPEv2(this);
    m_tagWMA=new KNMusicTagWma(this);
    m_tagM4A=new KNMusicTagM4A(this);
}

void KNMusicInfoCollector::analysis(const QString &filePath)
{   
    QFileInfo currentFile(filePath);
    if(!currentFile.exists())
    {
        emit requireSkipCurrent();
        return;
    }

    resetMusicInfo();
    KNMusicGlobal::MusicDetailsInfo currentFileInfo;
    currentFileInfo.filePath=currentFile.absoluteFilePath();
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
    QDateTime musicAddDate=QDateTime::currentDateTime();
    m_musicInfos[KNMusicGlobal::DateAdded]=
            musicAddDate.toString("yyyy-MM-dd APhh:mm");
    m_musicInfos[KNMusicGlobal::Kind]=
            m_musicGlobal->getDescription(m_musicGlobal->getMusicType(currentFile.suffix()));
    currentFileInfo.dateAdded=musicAddDate;

    readID3v1Tag(filePath);
    readAPEv2Tag(filePath);
    readID3v2Tag(filePath);
    readWMATag(filePath);
    readM4ATag(filePath);
    parseByMediaInfo(filePath);
    currentFileInfo.rating=m_musicRating;
    currentFileInfo.coverImage=m_musicCover;
    currentFileInfo.duration=m_duration;

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
    m_musicRating=0;
    m_musicCover=QPixmap();
}

void KNMusicInfoCollector::parseByMediaInfo(const QString &value)
{
    m_mediaInfo->analysisFile(value);
    QString mediaInfoData=m_mediaInfo->originalData();
    if(mediaInfoData.isEmpty())
    {
        return;
    }
    QStringList itemLines=mediaInfoData.split("\n"),
                basicInfo;
    QMap<QString, QString> basicInfoData;
    QString currentItem, itemCaption;
    int colonPosition, basicInfoIndex;
    bool audioBlock=false;
    basicInfo<<"Duration"
             <<"Bit rate"
             <<"Sampling rate";
    for(int i=0; i<itemLines.size(); i++)
    {
        currentItem=itemLines.at(i).simplified();
        colonPosition=currentItem.indexOf(':');
        if(colonPosition==-1)
        {
            audioBlock=(currentItem=="Audio");
            continue;
        }
        if(audioBlock)
        {
            itemCaption=currentItem.left(colonPosition).simplified();
            basicInfoIndex=basicInfo.indexOf(itemCaption);
            if(basicInfoIndex!=-1)
            {
                basicInfoData[basicInfo.takeAt(basicInfoIndex)]=
                        currentItem.mid(colonPosition+1).simplified();
            }
        }
    }

    QString rawInfoData;

    //Parse the datas.
    rawInfoData=basicInfoData["Duration"];
    int minutePos=rawInfoData.indexOf("mn"),
        secondPos=rawInfoData.indexOf("s");
    QString minuteString=rawInfoData.left(minutePos),
            secondString=rawInfoData.mid(minutePos+3, secondPos-minutePos-3);
    setMediaData(KNMusicGlobal::Time, minuteString+":"+secondString);
    m_duration=minuteString.toInt()*60+secondString.toInt();

    //Parse the bit rate.
    rawInfoData=basicInfoData["Bit rate"];
    secondPos=rawInfoData.lastIndexOf(" ");
    secondString=rawInfoData.left(secondPos);
    secondString.remove(' ');
    setMediaData(KNMusicGlobal::BitRate, secondString+" "+rawInfoData.mid(secondPos+1));
    m_bitRate=secondString.toFloat();

    //Parse the sampling rate.
    rawInfoData=basicInfoData["Sampling rate"];
    secondPos=rawInfoData.lastIndexOf(" ");
    secondString=rawInfoData.left(secondPos);
    secondString.remove(' ');
    setMediaData(KNMusicGlobal::SampleRate, secondString+" "+rawInfoData.mid(secondPos+1));
    m_samplingRate=secondString.toInt();
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
        setMediaData(KNMusicGlobal::TrackNumber,QString::number(id3v1Info.track));
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
            setMediaData(KNMusicGlobal::Description,m_tagID3v2->id3v2String("TIT3"));
            setMediaData(KNMusicGlobal::Genre,
                         m_musicGlobal->getGenre(m_tagID3v2->id3v2String("TCON")));
            setMediaData(KNMusicGlobal::Year,m_tagID3v2->id3v2String("TYER"));
            QString trackInfo=m_tagID3v2->id3v2String("TRCK");
            int diagonalPos=trackInfo.indexOf("/");
            if(diagonalPos!=-1)
            {
                setMediaData(KNMusicGlobal::TrackNumber,trackInfo.left(diagonalPos));
                setMediaData(KNMusicGlobal::TrackCount,trackInfo.mid(diagonalPos+1));
            }
            else
            {
                setMediaData(KNMusicGlobal::TrackNumber,trackInfo);
            }
            m_musicRating=m_tagID3v2->id3v2DataToRating("POPM");
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
            setMediaData(KNMusicGlobal::Description,m_tagID3v2->id3v2String("TT3"));
            setMediaData(KNMusicGlobal::Genre,
                         m_musicGlobal->getGenre(m_tagID3v2->id3v2String("TCO")));
            setMediaData(KNMusicGlobal::Year,m_tagID3v2->id3v2String("TYE"));
            QString trackInfo=m_tagID3v2->id3v2String("TRK");
            int diagonalPos=trackInfo.indexOf("/");
            if(diagonalPos!=-1)
            {
                setMediaData(KNMusicGlobal::TrackNumber,trackInfo.left(diagonalPos));
                setMediaData(KNMusicGlobal::TrackCount,trackInfo.mid(diagonalPos+1));
            }
            else
            {
                setMediaData(KNMusicGlobal::TrackNumber,trackInfo);
            }
            m_musicRating=m_tagID3v2->id3v2DataToRating("POP");
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
        setMediaData(KNMusicGlobal::Composer,m_tagAPEv2->tagStringData("Composer"));
        setMediaData(KNMusicGlobal::Genre,m_tagAPEv2->tagStringData("Genre"));
        setMediaData(KNMusicGlobal::Year,m_tagAPEv2->tagStringData("Year"));
        QString trackInfo=m_tagAPEv2->tagStringData("Track");
        setMediaData(KNMusicGlobal::TrackNumber,trackInfo);
    }
}

void KNMusicInfoCollector::readWMATag(const QString &value)
{
    if(m_tagWMA->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name, m_tagWMA->standardTag(KNMusicTagWma::WMA_FRAMEID_TITLE));
        setMediaData(KNMusicGlobal::Artist, m_tagWMA->standardTag(KNMusicTagWma::WMA_FRAMEID_AUTHOR));
        setMediaData(KNMusicGlobal::AlbumArtist, m_tagWMA->tagStringData("WM/AlbumArtist"));
        setMediaData(KNMusicGlobal::Album, m_tagWMA->tagStringData("WM/AlbumTitle"));
        setMediaData(KNMusicGlobal::BeatsPerMinuate, m_tagWMA->tagStringData("WM/BeatsPerMinute"));
        setMediaData(KNMusicGlobal::Comments, m_tagWMA->tagStringData("WM/Text"));
        setMediaData(KNMusicGlobal::Composer, m_tagWMA->tagStringData("WM/Composer"));
        setMediaData(KNMusicGlobal::Description, m_tagWMA->standardTag(KNMusicTagWma::WMA_FRAMEID_DESCRIPTION));
        setMediaData(KNMusicGlobal::Genre, m_tagWMA->tagStringData("WM/Genre"));
        setMediaData(KNMusicGlobal::Year, m_tagWMA->tagStringData("WM/Year"));
        QString track=m_tagWMA->tagStringData("WM/TrackNumber");
        setMediaData(KNMusicGlobal::TrackNumber,track);
    }
}

void KNMusicInfoCollector::readM4ATag(const QString &value)
{
    if(m_tagM4A->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name, m_tagM4A->metaData(KNMusicTagM4A::Title));
        setMediaData(KNMusicGlobal::Artist, m_tagM4A->metaData(KNMusicTagM4A::Artist));
        setMediaData(KNMusicGlobal::Album, m_tagM4A->metaData(KNMusicTagM4A::Album));
        setMediaData(KNMusicGlobal::AlbumArtist, m_tagM4A->metaData(KNMusicTagM4A::AlbumArtist));
        setMediaData(KNMusicGlobal::BeatsPerMinuate, m_tagM4A->metaData(KNMusicTagM4A::BPM));
        setMediaData(KNMusicGlobal::Genre, m_tagM4A->metaData(KNMusicTagM4A::Genre));
        setMediaData(KNMusicGlobal::Year, m_tagM4A->metaData(KNMusicTagM4A::Year));
        setMediaData(KNMusicGlobal::Composer, m_tagM4A->metaData(KNMusicTagM4A::Composer));
        setMediaData(KNMusicGlobal::Comments, m_tagM4A->metaData(KNMusicTagM4A::Comment));
        QByteArray trackData=m_tagM4A->metaData(KNMusicTagM4A::Tracknumber);
        setMediaData(KNMusicGlobal::TrackNumber, QString::number(trackData.at(3)));
        setMediaData(KNMusicGlobal::TrackCount, QString::number(trackData.at(5)));
        m_musicCover=m_tagM4A->albumArt();
    }
}

void KNMusicInfoCollector::setMediaData(const int &index,
                                        const QString &value)
{
    if(!value.isEmpty())
    {
        m_musicInfos[index]=value;
    }
}
