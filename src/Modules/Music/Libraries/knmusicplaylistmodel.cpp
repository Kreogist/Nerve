#include <QList>
#include <QStandardItem>
#include <QModelIndexList>
#include <QDataStream>
#include <QFileInfo>
#include <QDateTime>
#include <QBuffer>
#include <QTimer>
#include <QStringList>

#include <QTime>

#include <QDebug>

#include "../../Base/knlibdatabase.h"
#include "../../knglobal.h"
#include "knmusicstarrating.h"

#include "knmusicinfocollectormanager.h"

#include "knmusicplaylistmodel.h"

KNMusicPlaylistModel::KNMusicPlaylistModel(QObject *parent) :
    KNModel(parent)
{
    //Initial the music global.
    m_musicGlobal=KNMusicGlobal::instance();

    //Reset header data.
    resetHeader();
}

void KNMusicPlaylistModel::resetHeader()
{
    //Set the header text.
    QStringList header;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        header<<(m_musicGlobal->getHeader(i));
    }
    setHorizontalHeaderLabels(header);
    //Set header size hint.
    setHeaderData(0, Qt::Horizontal, QSize(10,23), Qt::SizeHintRole);
    //Set header alignment
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        setHeaderData(i, Qt::Horizontal, Qt::AlignVCenter, Qt::TextAlignmentRole);
    }
    //Set special header data, e.g.: Sort flag.
    setHeaderData(KNMusicGlobal::Time, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(KNMusicGlobal::Size, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(KNMusicGlobal::TrackNumber, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
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

QString KNMusicPlaylistModel::filePathFromIndex(const QModelIndex &index)
{
    return data(this->index(index.row(), KNMusicGlobal::Name), FilePathRole).toString();
}

QString KNMusicPlaylistModel::filePathFromIndex(const int &index)
{
    return data(this->index(index, KNMusicGlobal::Name), FilePathRole).toString();
}

QModelIndex KNMusicPlaylistModel::indexFromFilePath(const QString &filePath)
{
    QModelIndexList fileCheck=match(index(0,0),
                                    FilePathRole,
                                    filePath);
    return fileCheck.isEmpty()?QModelIndex():fileCheck.first();
}

QString KNMusicPlaylistModel::itemText(const int &row, const int &column) const
{
    return itemRoleData(row, column, Qt::DisplayRole).toString();
}

QVariant KNMusicPlaylistModel::itemRoleData(int row, int column, int role) const
{
    return data(index(row, column), role);
}

QList<QStandardItem *> KNMusicPlaylistModel::songRow(const int &row) const
{
    QList<QStandardItem *> songRowData;
    if(row>-1 && row<rowCount())
    {
        for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
        {
            songRowData.append(item(row, i)->clone());
        }
    }
    return songRowData;
}

void KNMusicPlaylistModel::addRawFileItem(QString filePath)
{
    QList<QStandardItem *> songItemList;
    QStandardItem *songItem;
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        songItem=new QStandardItem();
        songItem->setEditable(false);
        songItemList.append(songItem);
    }
    QFileInfo rawFileInfo(filePath);
    QDateTime currentTime=QDateTime::currentDateTime();
    songItem=songItemList.at(KNMusicGlobal::DateAdded);
    songItem->setData(currentTime, Qt::UserRole);
    songItem=songItemList.at(KNMusicGlobal::Name);
    songItem->setText(rawFileInfo.fileName());
    songItem->setData(filePath, FilePathRole);
    songItem->setData(1);
    appendRow(songItemList);
    updateIndexInfo(songItem->index(), filePath);
}

void KNMusicPlaylistModel::addRawFileItems(QStringList fileList)
{
    int listFileCount=fileList.size();
    while(listFileCount--)
    {
        addRawFileItem(fileList.takeFirst());
    }
}

void KNMusicPlaylistModel::setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager)
{
    KNModel::setInfoCollectorManager(infoCollectorManager);
    m_infoCollectorManager=qobject_cast<KNMusicInfoCollectorManager *>(infoCollectorManager);
}

void KNMusicPlaylistModel::retranslate()
{
    ;
}

void KNMusicPlaylistModel::retranslateAndSet()
{
    KNModel::retranslate();
}

void KNMusicPlaylistModel::onActionUpdateRowInfo()
{
    QStringList currentText=m_infoCollectorManager->currentFileData();
    KNMusicGlobal::MusicDetailsInfo currentDetails=
            m_infoCollectorManager->currentFileAppendData();
    int currentRow=m_infoCollectorManager->currentIndex();
    QStandardItem *songItem;
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        songItem=item(currentRow, i);
        songItem->setText(currentText.at(i));
    }
    setMusicDetailsInfo(currentRow, currentDetails);
    songItem=item(currentRow,KNMusicGlobal::Name);
    songItem->setData(currentDetails.filePath, FilePathRole);
    if(songItem->data().toInt()==1)
    {
        //This is a new file, never add to list.
        songItem->setData(0);
    }
    m_infoCollectorManager->removeFirstUpdateResult();
    if(!m_infoCollectorManager->isUpdateQueueEmpty())
    {
        emit requireUpdateNextRow();
    }
}

void KNMusicPlaylistModel::setMusicDetailsInfo(const int &currentRow,
                                       const KNMusicGlobal::MusicDetailsInfo &currentDetails)
{
    QStandardItem *songItem;
    songItem=item(currentRow,KNMusicGlobal::TrackNumber);
    songItem->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    songItem=item(currentRow,KNMusicGlobal::Time);
    songItem->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    songItem->setData(currentDetails.duration, Qt::UserRole);
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
    songItem->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    songItem=item(currentRow,KNMusicGlobal::Rating);
    songItem->setData(QVariant::fromValue(KNMusicStarRating(currentDetails.rating)),
                      0);
    songItem->setEditable(true);
    songItem=item(currentRow,KNMusicGlobal::DateAdded);
    songItem->setText(KNGlobal::instance()->dateTimeToDisplayString(
                          songItem->data(Qt::UserRole).toDateTime()));
}

void KNMusicPlaylistModel::updateIndexInfo(const QModelIndex &index,
                                   const QString &filePath)
{
    infoCollectorManager()->addAnalysisList(index.row(),
                                            filePath);
}

