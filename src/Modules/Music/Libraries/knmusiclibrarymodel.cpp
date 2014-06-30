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
#include "../../Base/knlibhashpixmaplist.h"
#include "../../knglobal.h"

#include "knmusicinfocollectormanager.h"

#include "knmusiclibrarymodel.h"

KNMusicLibraryModel::KNMusicLibraryModel(QObject *parent) :
    KNMusicLibraryModelBase(parent)
{
    //Initial the music global.
    m_musicGlobal=KNMusicGlobal::instance();

    //Reset header data.
    resetHeader();

    //The album art process list.
    //All the album art data will get from here.
    m_pixmapList=new KNLibHashPixmapList;
    m_pixmapList->moveToThread(&m_pixmapListThread);
    connect(m_pixmapList, &KNLibHashPixmapList::requireUpdatePixmap,
            this, &KNMusicLibraryModel::onActionUpdatePixmap);
    connect(m_pixmapList, &KNLibHashPixmapList::loadComplete,
            this, &KNMusicLibraryModel::onActionImageLoadComplete);
    connect(this, &KNMusicLibraryModel::requireLoadImage,
            m_pixmapList, &KNLibHashPixmapList::loadImages);

    //Start pixmap list thread.
    m_pixmapListThread.start();
}

KNMusicLibraryModel::~KNMusicLibraryModel()
{
    m_pixmapListThread.quit();
    m_pixmapListThread.wait();

    m_pixmapList->deleteLater();
}

QImage KNMusicLibraryModel::artwork(const int &row) const
{
    QString imageKey=artworkKey(row);
    return imageKey.isEmpty()?m_musicGlobal->noAlbumImage():artworkFromKey(imageKey);
}

QImage KNMusicLibraryModel::artworkFromKey(const QString &key) const
{
    QImage pixmap=m_pixmapList->pixmap(key);
    return pixmap.isNull()?m_musicGlobal->noAlbumImage():pixmap;
}

QString KNMusicLibraryModel::artworkKey(const int &row) const
{
    return data(index(row, KNMusicGlobal::Name),
                KNMusicGlobal::ArtworkKeyRole).toString();
}

void KNMusicLibraryModel::addRawFileItem(QString filePath)
{
    QModelIndexList fileCheck=match(index(0,0),
                                    KNMusicGlobal::FilePathRole,
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
    songItem->setData(filePath, KNMusicGlobal::FilePathRole);
    songItem->setData(1);
    appendRow(songItemList);
    m_rawFileCount++;
    updateIndexInfo(songItem->index(), filePath);
}

void KNMusicLibraryModel::addRawFileItems(QStringList fileList)
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

void KNMusicLibraryModel::setAlbumArtPath(const QString &path)
{
    m_pixmapList->setAlbumArtPath(path);
    emit requireLoadImage();
}

void KNMusicLibraryModel::recoverFile(QStringList textList,
                               KNMusicGlobal::MusicDetailsInfo currentDetails)
{
    emit musicRecover(appendMusicItem(textList, currentDetails));
}

void KNMusicLibraryModel::setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager)
{
    KNModel::setInfoCollectorManager(infoCollectorManager);
    m_infoCollectorManager=qobject_cast<KNMusicInfoCollectorManager *>(infoCollectorManager);
}

void KNMusicLibraryModel::retranslate()
{
    ;
}

void KNMusicLibraryModel::retranslateAndSet()
{
    KNModel::retranslate();
}

void KNMusicLibraryModel::onActionRecoverComplete()
{
    m_dataRecoverComplete=true;
    emit requireResort();
    if(m_imageRecoverComplete)
    {
        emit requireUpdateImage();
    }
}

void KNMusicLibraryModel::onActionUpdateRowInfo()
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
    if(!currentDetails.coverImage.isNull())
    {
        m_pixmapList->appendImage(currentRow, currentDetails.coverImage);
    }
    songItem=item(currentRow,KNMusicGlobal::Name);
    songItem->setData(currentDetails.filePath, KNMusicGlobal::FilePathRole);
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

void KNMusicLibraryModel::onActionImageLoadComplete()
{
    m_imageRecoverComplete=true;
    if(m_dataRecoverComplete)
    {
        emit requireUpdateImage();
    }
}

void KNMusicLibraryModel::onActionUpdatePixmap()
{
    setData(index(m_pixmapList->currentRow(),
                  KNMusicGlobal::Name),
            m_pixmapList->currentKey(),
            KNMusicGlobal::ArtworkKeyRole);
    emit musicAlbumArtUpdate(m_pixmapList->currentRow());
    m_pixmapList->removeCurrentUpdate();
}

void KNMusicLibraryModel::updateIndexInfo(const QModelIndex &index,
                                   const QString &filePath)
{
    infoCollectorManager()->addAnalysisList(index.row(),
                                            filePath);
}

void KNMusicLibraryModel::removeAppendData(const QModelIndex &removedIndex)
{
    //Here, we have to remove the image first.
    QString currentKey=artworkKey(removedIndex.row());
    QModelIndexList artworkCheck=match(index(0,0),
                                       KNMusicGlobal::ArtworkKeyRole,
                                       currentKey);
    if(artworkCheck.size()==1)
    {
        m_pixmapList->removeImage(currentKey);
    }
    //Emit the data about to remove signal.
    emit musicAboutToRemove(removedIndex);
}
