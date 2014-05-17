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
#include "knmusictagflac.h"

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
    m_tagFLAC=new KNMusicTagFLAC(this);
}

void KNMusicInfoCollector::analysis(const QString &filePath)
{   
    //I don't know why, but use a new QString can avoid a crash bug.
    QString filePathBackup=filePath;
    QFileInfo currentFile(filePathBackup);
    if(!currentFile.exists())
    {
        emit requireSkipCurrent();
        return;
    }

    resetInfoCache();
    KNMusicGlobal::MusicDetailsInfo currentFileInfo;
    currentFileInfo.filePath=currentFile.absoluteFilePath();
    m_musicInfos[KNMusicGlobal::Name]=currentFile.fileName();
    m_musicInfos[KNMusicGlobal::Size]=
            m_global->byteToHigher(currentFile.size());
    currentFileInfo.size=currentFile.size();
    m_musicInfos[KNMusicGlobal::DateModified]=
            m_global->dateTimeToDisplayString(currentFile.lastModified());
    currentFileInfo.dateModified=currentFile.lastModified();
    m_musicInfos[KNMusicGlobal::LastPlayed]=
            m_global->dateTimeToDisplayString(currentFile.lastRead());
    currentFileInfo.lastPlayed=currentFile.lastRead();
    m_musicInfos[KNMusicGlobal::Kind]=
            m_musicGlobal->getDescription(m_musicGlobal->getMusicType(currentFile.suffix()));

    readID3v1Tag(filePathBackup);
    readAPEv2Tag(filePathBackup);
    readID3v2Tag(filePathBackup);
    readWMATag(filePathBackup);
    readM4ATag(filePathBackup);
    readFLACTag(filePathBackup);
    parseByMediaInfo(filePathBackup);

    currentFileInfo.rating=m_musicRating;
    currentFileInfo.duration=m_duration;
    currentFileInfo.bitRate=m_bitRate;
    currentFileInfo.samplingRate=m_samplingRate;

    currentFileInfo.coverImage=m_musicCover;

    QStringList musicInfo;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        musicInfo.append(m_musicInfos[i]);
    }

    emit requireAppendMusic(musicInfo,
                            currentFileInfo);
}

void KNMusicInfoCollector::resetInfoCache()
{
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        m_musicInfos[i].clear();
    }
    m_musicRating=0;
    m_duration=0;
    m_bitRate=0;
    m_samplingRate=0;
    m_musicCover=QImage();
}

void KNMusicInfoCollector::parseByMediaInfo(const QString &value)
{
    m_mediaInfo->analysisFile(value);
    QString mediaInfoData=m_mediaInfo->originalData();
    if(mediaInfoData.isEmpty())
    {
        return;
    }
    QStringList itemLines, basicInfo;
#ifdef Q_OS_WIN32
    itemLines=mediaInfoData.split("\n");
#endif
#ifdef Q_OS_MACX
    itemLines=mediaInfoData.split("\r");
#endif
#ifdef Q_OS_LINUX
    itemLines=mediaInfoData.split("\n");
#endif
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
        if(currentItem.isEmpty())
        {
            continue;
        }
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
    int hourPos=rawInfoData.indexOf("h"),
        minutePos=rawInfoData.indexOf("mn"),
        secondPos=rawInfoData.indexOf("s"),
        hour, minute, second;
    if(hourPos==-1)
    {
        if(minutePos==-1)
        {
            //Too short.
            m_duration=rawInfoData.left(secondPos).toInt();
            setMediaData(KNMusicGlobal::Time, "0:"+QString::number(m_duration));
        }
        else
        {
            minute=rawInfoData.left(minutePos).toInt();
            second=rawInfoData.mid(minutePos+3, secondPos-minutePos-3).toInt();
            m_duration=minute*60+second;
            setMediaData(KNMusicGlobal::Time, QString::number(minute)+
                                              ":"+
                                              QString::number(second));
        }
    }
    else
    {
        //So long.
        hour=rawInfoData.left(hourPos).toInt();
        minute=rawInfoData.mid(hourPos+2, minutePos-hourPos-2).toInt();
        second=rawInfoData.mid(minutePos+3, secondPos-minutePos-3).toInt();
        //Calculate minuate
        m_duration=hour*60+minute;
        setMediaData(KNMusicGlobal::Time, QString::number(m_duration)+
                                          ":"+
                                          QString::number(second));
        //Calculate second
        m_duration=m_duration*60+second;
    }

    QString numberData;
    //Parse the bit rate.
    rawInfoData=basicInfoData["Bit rate"];
    secondPos=rawInfoData.lastIndexOf(" ");
    numberData=rawInfoData.left(secondPos);
    numberData.remove(' ');
    setMediaData(KNMusicGlobal::BitRate, numberData+" "+rawInfoData.mid(secondPos+1));
    m_bitRate=numberData.toFloat();

    //Parse the sampling rate.
    rawInfoData=basicInfoData["Sampling rate"];
    secondPos=rawInfoData.lastIndexOf(" ");
    numberData=rawInfoData.left(secondPos);
    numberData.remove(' ');
    setMediaData(KNMusicGlobal::SampleRate, numberData+" "+rawInfoData.mid(secondPos+1));
    m_samplingRate=numberData.toFloat();
}

void KNMusicInfoCollector::readID3v1Tag(const QString &value)
{
    if(m_tagID3v1->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name       ,m_tagID3v1->textData(KNMusicTagID3v1::Title));
        setMediaData(KNMusicGlobal::Artist     ,m_tagID3v1->textData(KNMusicTagID3v1::Artist));
        setMediaData(KNMusicGlobal::Album      ,m_tagID3v1->textData(KNMusicTagID3v1::Album));
        setMediaData(KNMusicGlobal::Genre      ,m_tagID3v1->textData(KNMusicTagID3v1::Genre));
        setMediaData(KNMusicGlobal::Year       ,m_tagID3v1->textData(KNMusicTagID3v1::Year));
        setMediaData(KNMusicGlobal::Comments   ,m_tagID3v1->textData(KNMusicTagID3v1::Comment));
        setMediaData(KNMusicGlobal::TrackNumber,m_tagID3v1->textData(KNMusicTagID3v1::Track));
    }
}

void KNMusicInfoCollector::readID3v2Tag(const QString &value)
{
    if(m_tagID3v2->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name            ,m_tagID3v2->textData(KNMusicTagID3v2::Name));
        setMediaData(KNMusicGlobal::Artist          ,m_tagID3v2->textData(KNMusicTagID3v2::Artist));
        setMediaData(KNMusicGlobal::Album           ,m_tagID3v2->textData(KNMusicTagID3v2::Album));
        setMediaData(KNMusicGlobal::AlbumArtist     ,m_tagID3v2->textData(KNMusicTagID3v2::AlbumArtist));
        setMediaData(KNMusicGlobal::BeatsPerMinuate ,m_tagID3v2->textData(KNMusicTagID3v2::BeatsPerMinuate));
        setMediaData(KNMusicGlobal::Category        ,m_tagID3v2->textData(KNMusicTagID3v2::Category));
        setMediaData(KNMusicGlobal::Composer        ,m_tagID3v2->textData(KNMusicTagID3v2::Composer));
        setMediaData(KNMusicGlobal::Description     ,m_tagID3v2->textData(KNMusicTagID3v2::Description));
        setMediaData(KNMusicGlobal::Year            ,m_tagID3v2->textData(KNMusicTagID3v2::Year));
        setMediaData(KNMusicGlobal::Genre,
                     m_musicGlobal->getGenre(m_tagID3v2->textData(KNMusicTagID3v2::Genre)));
        QString trackInfo=m_tagID3v2->textData(KNMusicTagID3v2::Track);
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
        trackInfo=m_tagID3v2->textData(KNMusicTagID3v2::Disc);
        diagonalPos=trackInfo.indexOf("/");
        if(diagonalPos!=-1)
        {
            setMediaData(KNMusicGlobal::DiscNumber,trackInfo.left(diagonalPos));
            setMediaData(KNMusicGlobal::DiscCount,trackInfo.mid(diagonalPos+1));
        }
        else
        {
            setMediaData(KNMusicGlobal::DiscNumber,trackInfo);
        }
        m_musicRating=m_tagID3v2->id3v2RatingData();
        setMusicCover(m_tagID3v2->tagImage(3)); //3 is the Cover front.
        if(m_musicCover.isNull())
        {
            setMusicCover(m_tagID3v2->firstAvaliableImage());
        }
    }
}

void KNMusicInfoCollector::readAPEv2Tag(const QString &value)
{
    if(m_tagAPEv2->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name        ,m_tagAPEv2->textData(KNMusicTagAPEv2::Name));
        setMediaData(KNMusicGlobal::Artist      ,m_tagAPEv2->textData(KNMusicTagAPEv2::Artist));
        setMediaData(KNMusicGlobal::Album       ,m_tagAPEv2->textData(KNMusicTagAPEv2::Album));
        setMediaData(KNMusicGlobal::Comments    ,m_tagAPEv2->textData(KNMusicTagAPEv2::Comments));
        setMediaData(KNMusicGlobal::Composer    ,m_tagAPEv2->textData(KNMusicTagAPEv2::Composer));
        setMediaData(KNMusicGlobal::Genre       ,m_tagAPEv2->textData(KNMusicTagAPEv2::Genre));
        setMediaData(KNMusicGlobal::Year        ,m_tagAPEv2->textData(KNMusicTagAPEv2::Year));
        setMediaData(KNMusicGlobal::TrackNumber ,m_tagAPEv2->textData(KNMusicTagAPEv2::Track));
    }
}

void KNMusicInfoCollector::readWMATag(const QString &value)
{
    if(m_tagWMA->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name           ,m_tagWMA->textData(KNMusicTagWma::Name));
        setMediaData(KNMusicGlobal::Artist         ,m_tagWMA->textData(KNMusicTagWma::Artist));
        setMediaData(KNMusicGlobal::AlbumArtist    ,m_tagWMA->textData(KNMusicTagWma::AlbumArtist));
        setMediaData(KNMusicGlobal::Album          ,m_tagWMA->textData(KNMusicTagWma::Album));
        setMediaData(KNMusicGlobal::BeatsPerMinuate,m_tagWMA->textData(KNMusicTagWma::BeatsPerMinuate));
        setMediaData(KNMusicGlobal::Comments       ,m_tagWMA->textData(KNMusicTagWma::Comments));
        setMediaData(KNMusicGlobal::Composer       ,m_tagWMA->textData(KNMusicTagWma::Composer));
        setMediaData(KNMusicGlobal::Description    ,m_tagWMA->textData(KNMusicTagWma::Description));
        setMediaData(KNMusicGlobal::Genre          ,m_tagWMA->textData(KNMusicTagWma::Genre));
        setMediaData(KNMusicGlobal::Year           ,m_tagWMA->textData(KNMusicTagWma::Year));
        setMediaData(KNMusicGlobal::TrackNumber    ,m_tagWMA->textData(KNMusicTagWma::TrackNumber));
        setMusicCover(m_tagWMA->albumArt());
    }
}

void KNMusicInfoCollector::readM4ATag(const QString &value)
{
    if(m_tagM4A->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name,           m_tagM4A->textData(KNMusicTagM4A::Title));
        setMediaData(KNMusicGlobal::Artist,         m_tagM4A->textData(KNMusicTagM4A::Artist));
        setMediaData(KNMusicGlobal::Album,          m_tagM4A->textData(KNMusicTagM4A::Album));
        setMediaData(KNMusicGlobal::AlbumArtist,    m_tagM4A->textData(KNMusicTagM4A::AlbumArtist));
        setMediaData(KNMusicGlobal::BeatsPerMinuate,m_tagM4A->textData(KNMusicTagM4A::BPM));
        setMediaData(KNMusicGlobal::Category,       m_tagM4A->textData(KNMusicTagM4A::Category));
        setMediaData(KNMusicGlobal::Composer,       m_tagM4A->textData(KNMusicTagM4A::Composer));
        setMediaData(KNMusicGlobal::Comments,       m_tagM4A->textData(KNMusicTagM4A::Comment));
        setMediaData(KNMusicGlobal::Description,    m_tagM4A->textData(KNMusicTagM4A::Description));
        setMediaData(KNMusicGlobal::Genre,          m_tagM4A->textData(KNMusicTagM4A::Genre));
        setMediaData(KNMusicGlobal::Grouping,       m_tagM4A->textData(KNMusicTagM4A::Grouping));
        setMediaData(KNMusicGlobal::Year,           m_tagM4A->textData(KNMusicTagM4A::Year));
        QByteArray trackData=m_tagM4A->metaData(KNMusicTagM4A::Tracknumber);
        if(trackData.size()>6)
        {
            setMediaData(KNMusicGlobal::TrackNumber, QString::number(trackData.at(3)));
            setMediaData(KNMusicGlobal::TrackCount, QString::number(trackData.at(5)));
        }
        setMusicCover(m_tagM4A->albumArt());
    }
}

void KNMusicInfoCollector::readFLACTag(const QString &value)
{
    if(m_tagFLAC->readTag(value))
    {
        setMediaData(KNMusicGlobal::Name        ,m_tagFLAC->textData(KNMusicTagFLAC::Name));
        setMediaData(KNMusicGlobal::Artist      ,m_tagFLAC->textData(KNMusicTagFLAC::Artist));
        setMediaData(KNMusicGlobal::Album       ,m_tagFLAC->textData(KNMusicTagFLAC::Album));
        setMediaData(KNMusicGlobal::Genre       ,m_tagFLAC->textData(KNMusicTagFLAC::Genre));
        setMediaData(KNMusicGlobal::Description ,m_tagFLAC->textData(KNMusicTagFLAC::Description));
        setMediaData(KNMusicGlobal::Composer    ,m_tagFLAC->textData(KNMusicTagFLAC::Composer));
        setMediaData(KNMusicGlobal::DiscCount   ,m_tagFLAC->textData(KNMusicTagFLAC::DiscCount));
        setMediaData(KNMusicGlobal::DiscNumber  ,m_tagFLAC->textData(KNMusicTagFLAC::DiscNumber));
        setMediaData(KNMusicGlobal::Comments    ,m_tagFLAC->textData(KNMusicTagFLAC::Comments));
        setMediaData(KNMusicGlobal::AlbumArtist ,m_tagFLAC->textData(KNMusicTagFLAC::AlbumArtist));
        setMediaData(KNMusicGlobal::TrackCount  ,m_tagFLAC->textData(KNMusicTagFLAC::TrackCount));
        setMediaData(KNMusicGlobal::Year        ,m_tagFLAC->textData(KNMusicTagFLAC::Year).left(4));
        setMediaData(KNMusicGlobal::TrackNumber ,m_tagFLAC->textData(KNMusicTagFLAC::TrackNumber));
        setMusicCover(m_tagFLAC->tagImage(3));
        if(m_musicCover.isNull())
        {
            setMusicCover(m_tagFLAC->firstAvaliableImage());
        }
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

void KNMusicInfoCollector::setMusicCover(const QImage &pixmap)
{
    if(!pixmap.isNull())
    {
        m_musicCover=pixmap;
    }
}
