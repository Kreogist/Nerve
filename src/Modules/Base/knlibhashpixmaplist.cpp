#include <QCryptographicHash>
#include <QBuffer>
#include <QStringList>
#include <QDir>

#include <QDebug>

#include "knlibhashpixmaplist.h"

KNLibPixmapBuffer::KNLibPixmapBuffer(QObject *parent) :
    QObject(parent)
{
    m_buffer=new QBuffer(&m_pixmapCache, this);
}

void KNLibPixmapBuffer::cachePixmap(const QPixmap &pixmap)
{
    m_pixmap=pixmap;
    m_pixmapCache.clear();
    m_pixmap.save(m_buffer, "png");
    m_hashCache=QCryptographicHash::hash(m_pixmapCache,
                                         QCryptographicHash::Md4);
    m_hashData.clear();
    for(int i=0; i<m_hashCache.size(); i++)
    {
        m_hashData+=QString::number((quint8)m_hashCache.at(i), 16);
    }
    emit cacheComplete();
}

void KNLibPixmapBuffer::savePixmap()
{
    m_pixmap.save(m_folderPath + m_hashData + ".png");
    emit saveComplete();
}

QString KNLibPixmapBuffer::hashData() const
{
    return m_hashData;
}

void KNLibPixmapBuffer::setFolderPath(const QString &folderPath)
{
    QDir albumFolder(folderPath);
    m_folderPath=albumFolder.absolutePath() + "/";
    if(!albumFolder.exists())
    {
        albumFolder.mkpath(m_folderPath);
    }
    QStringList cacheFileList=albumFolder.entryList(QStringList("*.png"),
                                                    QDir::Files | QDir::NoDotAndDotDot);
    QPixmap readingCache;
    int fileCount=cacheFileList.size();
    QString fileName;
    while(fileCount--)
    {
        fileName=cacheFileList.takeFirst();
        readingCache.load(m_folderPath+fileName, "png");
        fileName.resize(fileName.length()-4);
        emit requireRecoverData(fileName,
                                readingCache);
    }
}

QPixmap KNLibPixmapBuffer::pixmap() const
{
    return m_pixmap;
}

KNLibHashPixmapList::KNLibHashPixmapList(QObject *parent) :
    QObject(parent)
{
    m_buffer=new KNLibPixmapBuffer;
    m_buffer->moveToThread(&m_bufferThread);
    connect(m_buffer, &KNLibPixmapBuffer::cacheComplete,
            this, &KNLibHashPixmapList::onActionCacheComplete);
    connect(m_buffer, &KNLibPixmapBuffer::saveComplete,
            this, &KNLibHashPixmapList::onActionSaveComplete);
    connect(m_buffer, &KNLibPixmapBuffer::requireRecoverData,
            this, &KNLibHashPixmapList::onActionRecoverData);
    connect(this, &KNLibHashPixmapList::requireCachePixmap,
            m_buffer, &KNLibPixmapBuffer::cachePixmap);
    m_bufferThread.start();
}

KNLibHashPixmapList::~KNLibHashPixmapList()
{
    m_bufferThread.quit();
    m_bufferThread.wait();
    m_buffer->deleteLater();
}

QPixmap KNLibHashPixmapList::pixmap(const QString &key) const
{
    if(key.isEmpty())
    {
        return QPixmap();
    }
    return m_list[key];
}

void KNLibHashPixmapList::append(const int rowIndex,
                                 const QPixmap pixmap)
{
    qDebug()<<"Start: append.";
    if(pixmap.isNull())
    {
        return;
    }
    AnalysisQueueItem analysisItem;
    analysisItem.row=rowIndex;
    analysisItem.pixmap=pixmap;
    m_analysisQueue.append(analysisItem);
    if(!m_working)
    {
        m_working=true;
        emit requireCachePixmap(m_analysisQueue.first().pixmap);
    }
}

void KNLibHashPixmapList::removeCurrentUpdate()
{
    m_updateQueue.removeFirst();
}

void KNLibHashPixmapList::setAlbumArtPath(const QString &path)
{
    m_buffer->setFolderPath(path);
}

int KNLibHashPixmapList::currentRow() const
{
    return m_updateQueue.first().row;
}

QString KNLibHashPixmapList::currentKey() const
{
    return m_updateQueue.first().key;
}

void KNLibHashPixmapList::onActionCacheComplete()
{
    qDebug()<<"Slots: Cache Compelte.";
    QString keyData=m_buffer->hashData();
    if(!m_list.contains(keyData))
    {
        m_list[keyData]=m_buffer->pixmap();
    }
    UpdateQueueItem updateItem;
    updateItem.row=m_analysisQueue.first().row;
    updateItem.key=keyData;
    m_updateQueue.append(updateItem);
    emit requireUpdatePixmap();
    m_analysisQueue.removeFirst();
    m_buffer->savePixmap();
}

void KNLibHashPixmapList::onActionSaveComplete()
{
    qDebug()<<"Slots: Save Compelte."<<endl<<"Check:"
              <<"m_analysisQueue is empty: "<<m_analysisQueue.isEmpty();
    if(m_analysisQueue.isEmpty())
    {
        m_working=false;
        return;
    }
    emit requireCachePixmap(m_analysisQueue.first().pixmap);
}

void KNLibHashPixmapList::onActionRecoverData(const QString &key,
                                              const QPixmap &pixmap)
{
    m_list[key]=pixmap;
}
