#include <QDebug>

#include "knmusicinfocollector.h"

#include "knmusicinfocollectormanager.h"

KNMusicInfoCollectorManager::KNMusicInfoCollectorManager(QObject *parent) :
    KNLibInfoCollectorManager(parent)
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

void KNMusicInfoCollectorManager::addAnalysisList(int index,
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

void KNMusicInfoCollectorManager::currentWorkDone(QStringList value,
                                                  KNMusicGlobal::MusicDetailsInfo datas)
{
    ResultQueueItem resultItem;
    resultItem.index=m_analysisQueue.first().index;
    resultItem.text=value;
    resultItem.details=datas;
    m_resultQueue.append(resultItem);
    if(m_noUpdating) //Before this item is add, the result queue is empty.
    {                //Emit a signal to let model know they should update.
        emit requireUpdateRowInfo();
        m_noUpdating=false;
    }
    analysisNext();
}

KNMusicGlobal::MusicDetailsInfo KNMusicInfoCollectorManager::currentFileAppendData() const
{
    return m_resultQueue.first().details;
}

int KNMusicInfoCollectorManager::currentIndex() const
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


