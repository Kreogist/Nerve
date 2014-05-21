#include <QCryptographicHash>
#include <QBuffer>
#include <QStringList>
#include <QDir>
#include <QDataStream>
#include <QImage>

#include <QDebug>

#include "knlibhashpixmaplist.h"

KNLibImageBuffer::KNLibImageBuffer(QObject *parent) :
    QObject(parent)
{
    ;
}

void KNLibImageBuffer::setAlbumArtPath(const QString &folderPath)
{
    QDir folder(folderPath);
    m_albumArtFolder=folder.absolutePath()+"/";
    if(!folder.exists())
    {
        folder.mkpath(m_albumArtFolder);
    }
}

QString KNLibImageBuffer::albumArtPath() const
{
    return m_albumArtFolder;
}

QString KNLibImageBuffer::hash() const
{
    return m_hash;
}

void KNLibImageBuffer::hashImage(const QImage &image)
{
    m_hash.clear();
    m_imageByteCache.clear();
    m_imageByteCache.append((char *)image.bits(), image.byteCount());
    m_hashResult=QCryptographicHash::hash(m_imageByteCache, QCryptographicHash::Md4);
    m_hash.clear();
    for(int i=0; i<m_hashResult.size(); i++)
    {
        m_hash+=QString::number((quint8)m_hashResult.at(i), 16);
    }
    m_imageCache=image;
    emit hashComplete();
}

void KNLibImageBuffer::saveImage()
{
    m_imageCache.save(m_albumArtFolder+m_hash+".png", "PNG");
    emit saveComplete();
}

KNLibHashPixmapList::KNLibHashPixmapList(QObject *parent) :
    QObject(parent)
{
    m_buffer=new KNLibImageBuffer;
    m_buffer->moveToThread(&m_bufferThread);

    connect(this, &KNLibHashPixmapList::requireCacheImage,
            m_buffer, &KNLibImageBuffer::hashImage);
    connect(m_buffer, &KNLibImageBuffer::hashComplete,
            this, &KNLibHashPixmapList::onActionHashComplete);
    connect(this, &KNLibHashPixmapList::requireSaveImage,
            m_buffer, &KNLibImageBuffer::saveImage);
    connect(m_buffer, &KNLibImageBuffer::saveComplete,
            this, &KNLibHashPixmapList::onActionSaveComplete);

    m_bufferThread.start();
}

KNLibHashPixmapList::~KNLibHashPixmapList()
{
    m_bufferThread.quit();
    m_bufferThread.wait();
    m_buffer->deleteLater();
}

QImage KNLibHashPixmapList::pixmap(const QString &key) const
{
    return m_list[key];
}

void KNLibHashPixmapList::append(const int rowIndex,
                                 const QImage pixmap)
{
    AnalysisQueueItem analysisItem;
    analysisItem.row=rowIndex;
    analysisItem.pixmap=pixmap;
    m_analysisQueue.append(analysisItem);
    if(!m_working)
    {
        m_working=true;
        emit requireCacheImage(m_analysisQueue.first().pixmap);
    }
}

void KNLibHashPixmapList::loadImages()
{
    QDir albumArt(m_folderPath);
    QStringList albumArtFiles=albumArt.entryList(QStringList("*.png"));
    int fileCount=albumArtFiles.size();
    QString currentFile;
    QImage imageCache;
    while(fileCount--)
    {
        currentFile=albumArtFiles.takeFirst();
        if(currentFile=="." || currentFile=="..")
        {
            continue;
        }
        imageCache=QImage(m_folderPath+currentFile);
        currentFile.resize(currentFile.size()-4);
        m_list[currentFile]=imageCache;
    }
    emit loadComplete();
}

void KNLibHashPixmapList::removeCurrentUpdate()
{
    m_updateQueue.removeFirst();
}

void KNLibHashPixmapList::setAlbumArtPath(const QString &path)
{
    m_buffer->setAlbumArtPath(path);
    m_folderPath=m_buffer->albumArtPath();
}

int KNLibHashPixmapList::currentRow() const
{
    return m_updateQueue.first().row;
}

QString KNLibHashPixmapList::currentKey() const
{
    return m_updateQueue.first().key;
}

void KNLibHashPixmapList::onActionHashComplete()
{
    QString currentKey=m_buffer->hash();
    m_needToSaveImage=!m_list.contains(currentKey);
    if(m_needToSaveImage)
    {
        m_list[currentKey]=m_analysisQueue.first().pixmap;
    }
    UpdateQueueItem updateItem;
    updateItem.row=m_analysisQueue.first().row;
    updateItem.key=currentKey;
    m_updateQueue.append(updateItem);
    emit requireUpdatePixmap();
    m_analysisQueue.removeFirst();
    if(m_needToSaveImage)
    {
        emit requireSaveImage();
    }
    else
    {
        onActionSaveComplete();
    }
}

void KNLibHashPixmapList::onActionSaveComplete()
{
    if(m_analysisQueue.isEmpty())
    {
        m_working=false;
        return;
    }
    emit requireCacheImage(m_analysisQueue.first().pixmap);
}

void KNLibHashPixmapList::onActionRecoverData(const QString &key,
                                              const QImage &pixmap)
{
    m_list[key]=pixmap;
}
