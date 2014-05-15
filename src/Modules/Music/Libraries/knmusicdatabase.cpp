#include "knmusicmodel.h"

#include "../knmusicglobal.h"
#include "../../knglobal.h"

#include <QDebug>

#include "knmusicdatabase.h"

KNMusicDatabase::KNMusicDatabase(QObject *parent) :
    KNStdLibDatabase(parent)
{
    m_global=KNGlobal::instance();
}

void KNMusicDatabase::setModel(QAbstractItemModel *model)
{
    m_model=static_cast<KNMusicModel *>(model);
    connect(m_model, SIGNAL(musicAppend(QModelIndex)),
            this, SLOT(onActionRowAppend(QModelIndex)));
}

void KNMusicDatabase::recoverData()
{
    int recoverCount=size(), wholeCache, minuate, second;
    float decimalCache;
    QString dateTimeStringCache;
    QDateTime dateTimeCache;
    for(int i=0; i<recoverCount; i++)
    {
        QStringList textList;
        KNMusicGlobal::MusicDetailsInfo currentDetails;
        QJsonObject currentSong=row(i);
        for(int j=0; j<KNMusicGlobal::MusicDataCount; j++)
        {
            textList.append("");
        }
        textList[KNMusicGlobal::Name]=currentSong["Name"].toString();
        textList[KNMusicGlobal::Artist]=currentSong["Artist"].toString();
        textList[KNMusicGlobal::Album]=currentSong["Album"].toString();
        textList[KNMusicGlobal::Genre]=currentSong["Genre"].toString();
        textList[KNMusicGlobal::AlbumArtist]=currentSong["AlbumArtist"].toString();
        textList[KNMusicGlobal::BeatsPerMinuate]=currentSong["BeatsPerMinuate"].toString();
        textList[KNMusicGlobal::BitRate]=currentSong["BitRateText"].toString();
        textList[KNMusicGlobal::Category]=currentSong["Category"].toString();
        textList[KNMusicGlobal::Comments]=currentSong["Comments"].toString();
        textList[KNMusicGlobal::Composer]=currentSong["Composer"].toString();
        textList[KNMusicGlobal::Description]=currentSong["Description"].toString();
        textList[KNMusicGlobal::Equalizer]=currentSong["Equalizer"].toString();
        textList[KNMusicGlobal::Grouping]=currentSong["Grouping"].toString();
        textList[KNMusicGlobal::Kind]=currentSong["Kind"].toString();
        textList[KNMusicGlobal::SampleRate]=currentSong["SampleRateText"].toString();
        textList[KNMusicGlobal::Year]=currentSong["Year"].toString();

        wholeCache=(int)currentSong["Time"].toDouble();
        currentDetails.duration=wholeCache;
        minuate=wholeCache/60;
        second=wholeCache-minuate*60;
        textList[KNMusicGlobal::Time]=QString::number(minuate)+":"+QString::number(second);

        wholeCache=(int)currentSong["Size"].toDouble();
        currentDetails.size=wholeCache;
        textList[KNMusicGlobal::Size]=m_global->byteToHigher(currentDetails.size);

        wholeCache=(int)currentSong["DiscCount"].toDouble();
        textList[KNMusicGlobal::DiscCount]=QString::number(wholeCache);

        wholeCache=(int)currentSong["DiscNumber"].toDouble();
        textList[KNMusicGlobal::DiscNumber]=QString::number(wholeCache);

        wholeCache=(int)currentSong["TrackCount"].toDouble();
        textList[KNMusicGlobal::TrackCount]=QString::number(wholeCache);

        wholeCache=(int)currentSong["TrackNumber"].toDouble();
        textList[KNMusicGlobal::TrackNumber]=QString::number(wholeCache);

        decimalCache=(float)currentSong["BitRate"].toDouble();
        currentDetails.bitRate=decimalCache;

        decimalCache=(float)currentSong["SampleRate"].toDouble();
        currentDetails.samplingRate=decimalCache;

        dateTimeStringCache=currentSong["DateModified"].toString();
        dateTimeCache=stringToDateTime(dateTimeStringCache);
        currentDetails.dateModified=dateTimeCache;
        textList[KNMusicGlobal::DateModified]=dateTimeCache.toString("yyyy-MM-dd APhh:mm");

        dateTimeStringCache=currentSong["LastPlayed"].toString();
        dateTimeCache=stringToDateTime(dateTimeStringCache);
        currentDetails.lastPlayed=dateTimeCache;
        textList[KNMusicGlobal::LastPlayed]=dateTimeCache.toString("yyyy-MM-dd APhh:mm");

        currentDetails.filePath=currentSong["FilePath"].toString();
        m_model->recoverFile(textList, currentDetails);
    }
}

void KNMusicDatabase::onActionRowAppend(QModelIndex index)
{
    int row=index.row();
    QJsonObject currentSong;
    currentSong["Name"]=m_model->itemText(row, KNMusicGlobal::Name);
    currentSong["Time"]=m_model->itemRoleData(row, KNMusicGlobal::Time, Qt::UserRole).toInt();
    currentSong["Artist"]=m_model->itemText(row, KNMusicGlobal::Artist);
    currentSong["Album"]=m_model->itemText(row, KNMusicGlobal::Album);
    currentSong["Genre"]=m_model->itemText(row, KNMusicGlobal::Genre);
    currentSong["AlbumArtist"]=m_model->itemText(row, KNMusicGlobal::AlbumArtist);
    currentSong["AlbumRating"]=m_model->itemText(row, KNMusicGlobal::AlbumRating);
    currentSong["BeatsPerMinuate"]=m_model->itemText(row, KNMusicGlobal::BeatsPerMinuate);
    currentSong["BitRate"]=m_model->itemRoleData(row, KNMusicGlobal::BitRate, Qt::UserRole).toFloat();
    currentSong["BitRateText"]=m_model->itemText(row, KNMusicGlobal::BitRate);
    currentSong["Category"]=m_model->itemText(row, KNMusicGlobal::Category);
    currentSong["Comments"]=m_model->itemText(row, KNMusicGlobal::Comments);
    currentSong["Composer"]=m_model->itemText(row, KNMusicGlobal::Composer);
    currentSong["DateModified"]=dateTimeToString(m_model->itemRoleData(row, KNMusicGlobal::DateModified,
                                                      Qt::UserRole).toDateTime());
    currentSong["Description"]=m_model->itemText(row, KNMusicGlobal::Description);
    currentSong["DiscCount"]=m_model->itemText(row, KNMusicGlobal::DiscCount).toInt();
    currentSong["DiscNumber"]=m_model->itemText(row, KNMusicGlobal::DiscNumber).toInt();
    currentSong["Equalizer"]=m_model->itemText(row, KNMusicGlobal::Equalizer);
    currentSong["Grouping"]=m_model->itemText(row, KNMusicGlobal::Grouping);
    currentSong["Kind"]=m_model->itemText(row, KNMusicGlobal::Kind);
    currentSong["LastPlayed"]=dateTimeToString(m_model->itemRoleData(row, KNMusicGlobal::LastPlayed,
                                                      Qt::UserRole).toDateTime());
    currentSong["SampleRate"]=m_model->itemRoleData(row, KNMusicGlobal::SampleRate, Qt::UserRole).toFloat();
    currentSong["SampleRateText"]=m_model->itemText(row, KNMusicGlobal::SampleRate);
    currentSong["Size"]=m_model->itemRoleData(row, KNMusicGlobal::Size, Qt::UserRole).toInt();
    currentSong["Time"]=m_model->itemRoleData(row, KNMusicGlobal::Time, Qt::UserRole).toInt();
    currentSong["TrackCount"]=m_model->itemText(row, KNMusicGlobal::TrackCount);
    currentSong["TrackNumber"]=m_model->itemText(row, KNMusicGlobal::TrackNumber);
    currentSong["Year"]=m_model->itemText(row, KNMusicGlobal::Year);

    currentSong["FilePath"]=m_model->itemRoleData(row, KNMusicGlobal::Name, Qt::UserRole).toString();
    append(currentSong);
    writeToDisk();
}

QString KNMusicDatabase::dateTimeToString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyyMMddHHmmss");
}

QDateTime KNMusicDatabase::stringToDateTime(const QString &string)
{
    return QDateTime::fromString(string, "yyyyMMddHHmmss");
}
