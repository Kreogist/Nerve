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
    connect(m_model, &KNMusicModel::musicAppend,
            this, &KNMusicDatabase::onActionRowAppend);
    connect(m_model, &KNMusicModel::musicAboutToRemove,
            this, &KNMusicDatabase::onActionRowRemove);
    connect(m_model, &KNMusicModel::musicUpdate,
            this, &KNMusicDatabase::onActionRowUpdate);
}

void KNMusicDatabase::recoverData()
{
    int recoverCount=size();
    QString dateTimeStringCache;
    QDateTime dateTimeCache;
    for(int i=0; i<recoverCount; i++)
    {
        QStringList textList;
        KNMusicGlobal::MusicDetailsInfo currentDetails;
        QJsonObject currentSong=row(i);
        QJsonArray displayText=currentSong["Text"].toArray();
        for(int j=0; j<KNMusicGlobal::MusicDataCount; j++)
        {
            textList.append(displayText.at(j).toString());
        }

        currentDetails.duration=(int)currentSong["Time"].toDouble();
        currentDetails.size=(int)currentSong["Size"].toDouble();
        currentDetails.bitRate=(float)currentSong["BitRate"].toDouble();
        currentDetails.samplingRate=(float)currentSong["SampleRate"].toDouble();

        dateTimeStringCache=currentSong["DateModified"].toString();
        dateTimeCache=stringToDateTime(dateTimeStringCache);
        currentDetails.dateModified=dateTimeCache;
        textList[KNMusicGlobal::DateModified]=dateTimeCache.toString("yyyy-MM-dd APhh:mm");

        dateTimeStringCache=currentSong["LastPlayed"].toString();
        dateTimeCache=stringToDateTime(dateTimeStringCache);
        currentDetails.lastPlayed=dateTimeCache;
        textList[KNMusicGlobal::LastPlayed]=dateTimeCache.toString("yyyy-MM-dd APhh:mm");

        dateTimeStringCache=currentSong["DateAdded"].toString();
        dateTimeCache=stringToDateTime(dateTimeStringCache);
        currentDetails.dateAdded=dateTimeCache;
        textList[KNMusicGlobal::DateAdded]=dateTimeCache.toString("yyyy-MM-dd APhh:mm");

        currentDetails.filePath=currentSong["FilePath"].toString();
        m_model->recoverFile(textList, currentDetails);
    }
}

void KNMusicDatabase::onActionRowAppend(const QModelIndex &index)
{
    append(createRowObject(index.row()));
}

void KNMusicDatabase::onActionRowUpdate(const QModelIndex &index)
{
    replace(index.row(), createRowObject(index.row()));
}

void KNMusicDatabase::onActionRowRemove(const QModelIndex &index)
{
    removeAt(index.row());
}

QJsonObject KNMusicDatabase::createRowObject(const int &row)
{
    QJsonObject currentSong;
    QJsonArray currentSongText;
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        currentSongText.append(m_model->itemText(row, i));
    }
    currentSong["Text"]=currentSongText;
    currentSong["Time"]=m_model->itemRoleData(row, KNMusicGlobal::Time, Qt::UserRole).toInt();
    currentSong["BitRate"]=m_model->itemRoleData(row, KNMusicGlobal::BitRate, Qt::UserRole).toFloat();
    currentSong["DateModified"]=dateTimeToString(m_model->itemRoleData(row, KNMusicGlobal::DateModified,
                                                      Qt::UserRole).toDateTime());
    currentSong["LastPlayed"]=dateTimeToString(m_model->itemRoleData(row, KNMusicGlobal::LastPlayed,
                                                      Qt::UserRole).toDateTime());
    currentSong["DateAdded"]=dateTimeToString(m_model->itemRoleData(row, KNMusicGlobal::DateAdded,
                                                      Qt::UserRole).toDateTime());
    currentSong["SampleRate"]=m_model->itemRoleData(row, KNMusicGlobal::SampleRate, Qt::UserRole).toFloat();
    currentSong["Size"]=m_model->itemRoleData(row, KNMusicGlobal::Size, Qt::UserRole).toInt();
    currentSong["Time"]=m_model->itemRoleData(row, KNMusicGlobal::Time, Qt::UserRole).toInt();
    currentSong["FilePath"]=m_model->itemRoleData(row, KNMusicGlobal::Name, Qt::UserRole).toString();
    return currentSong;
}

QString KNMusicDatabase::dateTimeToString(const QDateTime &dateTime)
{
    return dateTime.toString("yyyyMMddHHmmss");
}

QDateTime KNMusicDatabase::stringToDateTime(const QString &string)
{
    return QDateTime::fromString(string, "yyyyMMddHHmmss");
}
