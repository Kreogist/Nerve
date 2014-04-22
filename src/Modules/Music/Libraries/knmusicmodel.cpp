#include <QList>
#include <QStandardItem>
#include <QModelIndexList>
#include <QDataStream>
#include <QBuffer>

#include <QDebug>

#include "../../knglobal.h"
#include "knmusicstarrating.h"

#include "knmusicmodel.h"

KNMusicModel::KNMusicModel(QObject *parent) :
    KNModel(parent)
{
    musicGlobal=KNMusicGlobal::instance();
    QStringList header;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        header<<(musicGlobal->getHeader(i));
    }
    setHorizontalHeaderLabels(header);

    setHeaderData(KNMusicGlobal::TrackNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::Size, Qt::Horizontal, 2, Qt::UserRole);
}

bool KNMusicModel::readFromDataStream(QDataStream &stream)
{
    /*if(!stream.device()->isReadable())
    {
        return false;
    }
    char dataLength[4];
    stream.readRawData(dataLength, 4);
    quint32 dataLong=KNGlobal::charsToUint32(dataLength), i, j, songCount,
            rawPointer=4, itemLength;
    QByteArray byteCache;
    QString stringCache;
    char *rawData=new char[dataLong];
    stream.readRawData(rawData, dataLong);
    songCount=KNGlobal::charsToUint32(rawData);
    for(i=0; i<songCount; i++)
    {
        QStringList currentSong;
        KNMusicGlobal::MusicDetailsInfo currentData;
        for(j=0; j<KNMusicGlobal::MusicDataCount; j++)
        {
            itemLength=KNGlobal::charsToUint32(rawData+rawPointer);
            rawPointer+=4;
            currentSong<<QString::fromUtf8(rawData+rawPointer, itemLength);
            rawPointer+=itemLength;
        }

        itemLength=KNGlobal::charsToUint32(rawData+rawPointer);
        rawPointer+=4;
        stringCache=QByteArray(rawData+rawPointer, itemLength);
        currentData.dateModified=QDateTime::fromString(stringCache,
                                                       "yyyyMMddhhmmss");
        rawPointer+=itemLength;

        itemLength=KNGlobal::charsToUint32(rawData+rawPointer);
        rawPointer+=4;
        stringCache=QByteArray(rawData+rawPointer, itemLength);
        currentData.lastPlayed=QDateTime::fromString(stringCache,
                                                     "yyyyMMddhhmmss");
        rawPointer+=itemLength;

        itemLength=KNGlobal::charsToUint32(rawData+rawPointer);
        rawPointer+=4;
        currentData.filePath=QByteArray(rawData+rawPointer, itemLength);
        rawPointer+=itemLength;
        appendMusic(currentSong, currentData);
    }
    delete[] rawData;*/
    return true;
}

bool KNMusicModel::writeToDataStream(QDataStream &stream)
{
    /*if(!stream.device()->isWritable())
    {
        return false;
    }

    char lengthChar[4];
    QByteArray outputCache;
    QString stringCache;
    QByteArray byteCache, imageCache;
    QBuffer pixmapBuf(&imageCache);

    quint32 songCount=(quint32)rowCount(), i, j;
    KNGlobal::uint32ToChars(songCount, lengthChar);
    outputCache.append(lengthChar, 4);
    QStandardItem *songItem;
    for(i=0; i<songCount; i++)
    {
        for(j=0; j<KNMusicGlobal::MusicDataCount; j++)
        {
            songItem=item(i, j);
            byteCache.clear();
            byteCache.append(songItem->text());
            KNGlobal::uint32ToChars(byteCache.size(), lengthChar);
            outputCache.append(lengthChar, 4);
            outputCache.append(byteCache);
        }

        songItem=item(i, KNMusicGlobal::DateModified);
        stringCache=songItem->data(Qt::UserRole).toDateTime().toString("yyyyMMddhhmmss");
        byteCache.clear();
        byteCache.append(stringCache);
        KNGlobal::uint32ToChars(byteCache.size(), lengthChar);
        outputCache.append(lengthChar, 4);
        outputCache.append(byteCache);

        songItem=item(i, KNMusicGlobal::LastPlayed);
        stringCache=songItem->data(Qt::UserRole).toDateTime().toString("yyyyMMddhhmmss");
        byteCache.clear();
        byteCache.append(stringCache);
        KNGlobal::uint32ToChars(byteCache.size(), lengthChar);
        outputCache.append(lengthChar, 4);
        outputCache.append(byteCache);

        songItem=item(i, KNMusicGlobal::Name);
        byteCache.clear();
        byteCache.append(songItem->data(Qt::UserRole).toString());
        KNGlobal::uint32ToChars(byteCache.size(), lengthChar);
        outputCache.append(lengthChar, 4);
        outputCache.append(byteCache);
    }
    stream.writeBytes(outputCache.data(), outputCache.size());*/
    return true;
}

QString KNMusicModel::filePathFromIndex(const QModelIndex &index)
{
    return item(index.row(), KNMusicGlobal::Name)->data(Qt::UserRole).toString();
}

void KNMusicModel::appendMusic(const QStringList &info,
                               const KNMusicGlobal::MusicDetailsInfo &datas)
{
    QModelIndexList fileCheck=match(index(0,0),
                                    Qt::UserRole,
                                    datas.filePath);
    if(fileCheck.size()!=0)
    {
        //Find the same file in the model.
        return;
    }
    QList<QStandardItem *> songItemList;
    QStandardItem *songItem;
    for(int i=0;
        i<info.count();
        i++)
    {
        songItem=new QStandardItem(info.at(i));
        songItem->setEditable(false);
        songItemList.append(songItem);
    }
    songItem=songItemList.at(KNMusicGlobal::Time);
    songItem->setData(datas.coverImage, Qt::UserRole);
    songItem=songItemList.at(KNMusicGlobal::DateModified);
    songItem->setData(datas.dateModified, Qt::UserRole);
    songItem=songItemList.at(KNMusicGlobal::LastPlayed);
    songItem->setData(datas.lastPlayed, Qt::UserRole);
    songItem=songItemList.at(KNMusicGlobal::Size);
    songItem->setData(datas.size, Qt::UserRole);
    songItem=songItemList.at(KNMusicGlobal::Rating);
    songItem->setData(QVariant::fromValue(KNMusicStarRating(datas.rating)),
                      0);
    songItem->setEditable(true);

    songItem=songItemList.at(KNMusicGlobal::Name);
    songItem->setData(datas.filePath, Qt::UserRole);
    appendRow(songItemList);
    emit musicAppend(indexFromItem(songItem));
}

void KNMusicModel::retranslate()
{
    ;
}

void KNMusicModel::retranslateAndSet()
{
    KNModel::retranslate();
}
