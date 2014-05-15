#include <QList>
#include <QStandardItem>
#include <QModelIndexList>
#include <QDataStream>
#include <QFileInfo>
#include <QDateTime>
#include <QBuffer>
#include <QStringList>

#include <QTime>

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

QString KNMusicModel::filePathFromIndex(const QModelIndex &index)
{
    return item(index.row(), KNMusicGlobal::Name)->data(Qt::UserRole).toString();
}

QString KNMusicModel::itemText(const int &row, const int &column) const
{
    return data(index(row, column), Qt::DisplayRole).toString();
}

QVariant KNMusicModel::itemRoleData(int row, int column, int role) const
{
    return data(index(row, column), role);
}

QPixmap KNMusicModel::itemArtwork(const int &row) const
{
    return data(index(row, KNMusicGlobal::Time), Qt::UserRole+1).value<QPixmap>();
}

void KNMusicModel::addRawFileItem(QString filePath)
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
    QDateTime currentTime=QDateTime::currentDateTime();
    songItem=songItemList.at(KNMusicGlobal::DateAdded);
    songItem->setData(currentTime, Qt::UserRole);
    songItem=songItemList.at(KNMusicGlobal::Name);
    songItem->setText(rawFileInfo.fileName());
    songItem->setData(filePath, Qt::UserRole);
    songItem->setData(1);
    appendRow(songItemList);
    m_rawFileCount++;
    updateIndexInfo(songItem->index(), filePath);
}

void KNMusicModel::addRawFileItems(QStringList fileList)
{
    int listFileCount=fileList.size();
    //***Speed Test***
    qDebug()<<"***Speed Test***"<<endl<<"Files Count: "<<listFileCount;
    m_startTime=QTime::currentTime().msecsSinceStartOfDay();
    //****************
    while(listFileCount--)
    {
        addRawFileItem(fileList.takeFirst());
    }
}

void KNMusicModel::recoverFile(QStringList textList,
                               KNMusicGlobal::MusicDetailsInfo currentDetails)
{
    QList<QStandardItem *> songItemList;
    QStandardItem *songItem;
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        songItem=new QStandardItem(textList.at(i));
        songItem->setEditable(false);
        songItemList.append(songItem);
    }
    appendRow(songItemList);
    int currentRow=songItem->index().row();
    songItem=item(currentRow, KNMusicGlobal::DateAdded);
    songItem->setData(currentDetails.dateAdded, Qt::UserRole);
    setMusicDetailsInfo(currentRow,
                        currentDetails);
    songItem=item(currentRow, KNMusicGlobal::Name);
    songItem->setData(currentDetails.filePath, Qt::UserRole);
    emit musicRecover(songItem->index());
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

void KNMusicModel::onActionUpdateRowInfo()
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
    songItem->setData(currentDetails.filePath, Qt::UserRole);
    if(songItem->data().toInt()==1)
    {
        //This is a new file, never add to list.
        songItem->setData(0);
        m_rawFileCount--;
        emit musicAppend(songItem->index());
        if(m_rawFileCount==0)
        {
            //***Speed Test***
            qDebug()<<"Time Use(ms): "<<
                       QTime::currentTime().msecsSinceStartOfDay()-m_startTime;
            //****************
        }
    }
    else
    {
        emit musicUpdate(songItem->index());
    }
    m_infoCollectorManager->removeFirstUpdateResult();
    if(!m_infoCollectorManager->isUpdateQueueEmpty())
    {
        emit requireUpdateNextRow();
    }
}

void KNMusicModel::setMusicDetailsInfo(const int &currentRow,
                                       const KNMusicGlobal::MusicDetailsInfo &currentDetails)
{
    QStandardItem *songItem;
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
    songItem=item(currentRow,KNMusicGlobal::DateAdded);
    songItem->setText(KNGlobal::instance()->dateTimeToDisplayString(
                          songItem->data(Qt::UserRole).toDateTime()));
}

void KNMusicModel::updateIndexInfo(const QModelIndex &index,
                                   const QString &filePath)
{
    infoCollectorManager()->addAnalysisList(index.row(),
                                            filePath);
}

void KNMusicModel::prepareRemove(const QModelIndex &index)
{
    emit musicAboutToRemove(index);
}
