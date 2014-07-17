#include <QStandardItem>

#include <QDebug>

#include "knmusicinfocollector.h"

#include "knmusicinfocollectormanager.h"

KNMusicInfoCollectorManager::KNMusicInfoCollectorManager(QObject *parent) :
    KNMusicInfoCollectorManagerBase(parent)
{
    m_collector=new KNMusicInfoCollector;
    m_collector->moveToThread(&m_collectThread);
    connect(m_collector, &KNMusicInfoCollector::requireAppendMusic,
            this, &KNMusicInfoCollectorManager::currentWorkDone);
    connect(m_collector, &KNMusicInfoCollector::requireSkipCurrent,
            this, &KNMusicInfoCollectorManager::analysisNext);
    connect(this, &KNMusicInfoCollectorManager::requireAnalysis,
            m_collector, &KNMusicInfoCollector::analysis);
    m_collectThread.start();
}

KNMusicInfoCollectorManager::~KNMusicInfoCollectorManager()
{
    m_collectThread.quit();
    m_collectThread.wait();
    m_collector->deleteLater();
}

void KNMusicInfoCollectorManager::addAnalysisList(QStandardItem *index,
                                                  QString filePath)
{
    AnalysisQueueItem analysisItem;
    analysisItem.index=index;
    analysisItem.filePath=filePath;
    m_analysisQueue.append(analysisItem);
    if(!m_working)
    {
        m_working=true;
        emit requireAnalysis(m_analysisQueue.first().filePath);
    }
}

void KNMusicInfoCollectorManager::removedIndexesInList(QModelIndex removedIndex)
{
    for(int i=m_analysisQueue.size()-1;
        i>-1;
        i--)
    {
        if(m_analysisQueue.at(i).index->row()==removedIndex.row())
        {
            m_analysisQueue.removeAt(i);
        }
    }
    for(int i=m_resultQueue.size()-1;
        i>-1;
        i--)
    {
        if(m_resultQueue.at(i).index->row()==removedIndex.row())
        {
            m_resultQueue.removeAt(i);
        }
    }
}

void KNMusicInfoCollectorManager::analysisNext()
{
    m_analysisQueue.removeFirst();
    if(m_analysisQueue.size()==0)
    {
        m_working=false;
        return;
    }
    emit requireAnalysis(m_analysisQueue.first().filePath);
}

void KNMusicInfoCollectorManager::currentWorkDone(QString filePath,
                                                  QStringList value,
                                                  KNMusicGlobal::MusicDetailsInfo datas)
{
    QStandardItem *properItem=m_analysisQueue.first().index;
    if(properItem->data(KNMusicGlobal::FilePathRole).toString()==filePath)
    {
        ResultQueueItem resultItem;
        resultItem.index=properItem;
        resultItem.text=value;
        resultItem.details=datas;
        m_resultQueue.append(resultItem);
        if(m_noUpdating) //Before this item is add, the result queue is empty.
        {                //Emit a signal to let model know they should update.
            emit requireUpdateRowInfo();
            m_noUpdating=false;
        }
    }
    analysisNext();
}

KNMusicGlobal::MusicDetailsInfo KNMusicInfoCollectorManager::currentFileAppendData() const
{
    return m_resultQueue.first().details;
}

QStandardItem *KNMusicInfoCollectorManager::currentIndex() const
{
    return m_resultQueue.first().index;
}

void KNMusicInfoCollectorManager::removeFirstUpdateResult()
{
    m_resultQueue.removeFirst();
    if(m_resultQueue.isEmpty())
    {
        m_noUpdating=true;
    }
}

bool KNMusicInfoCollectorManager::isUpdateQueueEmpty()
{
    return m_resultQueue.isEmpty();
}

bool KNMusicInfoCollectorManager::isWorking()
{
    return !m_analysisQueue.isEmpty();
}

void KNMusicInfoCollectorManager::setMusicBackend(KNMusicBackend *backend)
{
    m_collector->setMusicBackend(backend);
}

QStringList KNMusicInfoCollectorManager::currentFileData() const
{
    return m_resultQueue.first().text;
}


