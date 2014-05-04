#include <QList>
#include <QStandardItem>
#include <QModelIndexList>
#include <QDataStream>
#include <QFileInfo>
#include <QBuffer>
#include <QStringList>

#include <QDebug>

#include "../../knglobal.h"
#include "knmusicstarrating.h"

#include "knmusicinfocollectormanager.h"

#include "knmusicmodel.h"

KNMusicModel::KNMusicModel(QObject *parent) :
    KNModel(parent)
{
    m_musicGlobal=KNMusicGlobal::instance();
    QStringList header;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        header<<(m_musicGlobal->getHeader(i));
    }
    setHorizontalHeaderLabels(header);

    setHeaderData(KNMusicGlobal::Time, Qt::Horizontal, 2, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DiscNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DiscCount, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::TrackNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::TrackCount, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::Size, Qt::Horizontal, 2, Qt::UserRole);
    setHeaderData(KNMusicGlobal::BitRate, Qt::Horizontal, 3, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DateAdded, Qt::Horizontal, 4, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DateModified, Qt::Horizontal, 4, Qt::UserRole);
    setHeaderData(KNMusicGlobal::LastPlayed, Qt::Horizontal, 4, Qt::UserRole);
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

void KNMusicModel::addRawFileItem(const QString &filePath)
{
    QModelIndexList fileCheck=match(index(0,0),
                                    Qt::UserRole,
                                    filePath);
    if(fileCheck.size()!=0)
    {
        //Find the same file in the model, update it's information.
        updateIndexInfo(fileCheck.at(0), filePath);
        return;
    }
    QList<QStandardItem *> songItemList;
    QStandardItem *songItem;
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        songItem=new QStandardItem();
        songItem->setEditable(false);
        songItemList.append(songItem);
    }
    QFileInfo rawFileInfo(filePath);
    songItem=songItemList.at(KNMusicGlobal::Name);
    songItem->setText(rawFileInfo.fileName());
    songItem->setData(filePath, Qt::UserRole);
    songItem->setData(1);
    appendRow(songItemList);
    updateIndexInfo(songItem->index(), filePath);
}

void KNMusicModel::setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager)
{
    KNModel::setInfoCollectorManager(infoCollectorManager);
    m_infoCollectorManager=qobject_cast<KNMusicInfoCollectorManager *>(infoCollectorManager);
}

void KNMusicModel::retranslate()
{
    ;
}

void KNMusicModel::retranslateAndSet()
{
    KNModel::retranslate();
}

void KNMusicModel::onActionUpdateRowInfo(const QModelIndex &index)
{
    QStringList currentText=m_infoCollectorManager->currentFileData();
    KNMusicGlobal::MusicDetailsInfo currentDetails=
            m_infoCollectorManager->currentFileAppendData();
    int currentRow=index.row();
    QStandardItem *songItem;
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        songItem=item(currentRow, i);
        songItem->setText(currentText.at(i));
    }
    songItem=item(currentRow,KNMusicGlobal::Time);
    songItem->setData(QVariant(Qt::AlignRight), Qt::TextAlignmentRole);
    songItem->setData(currentDetails.duration, Qt::UserRole);
    songItem->setData(currentDetails.coverImage, Qt::UserRole+1);
    songItem=item(currentRow,KNMusicGlobal::BitRate);
    songItem->setData(QVariant(currentDetails.bitRate), Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::SampleRate);
    songItem->setData(currentDetails.samplingRate, Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::DateModified);
    songItem->setData(currentDetails.dateModified, Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::LastPlayed);
    songItem->setData(currentDetails.lastPlayed, Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::Size);
    songItem->setData(currentDetails.size, Qt::UserRole);
    songItem->setData(QVariant(Qt::AlignRight), Qt::TextAlignmentRole);
    songItem=item(currentRow,KNMusicGlobal::Rating);
    songItem->setData(QVariant::fromValue(KNMusicStarRating(currentDetails.rating)),
                      0);
    songItem->setEditable(true);

    songItem=item(currentRow,KNMusicGlobal::Name);
    songItem->setData(currentDetails.filePath, Qt::UserRole);
    if(songItem->data().toInt()==1)
    {
        //This is a new file, never add to list.
        songItem->setData(0);
        emit musicAppend(indexFromItem(songItem));
    }
    else
    {
        emit musicDataUpdate(indexFromItem(songItem));
    }
}

void KNMusicModel::updateIndexInfo(const QModelIndex &index,
                                   const QString &filePath)
{
    infoCollectorManager()->addAnalysisList(index,
                                            filePath);
}
