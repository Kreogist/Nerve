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
            this, &KNMusicInfoCollectorManager::currentSkip);
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

void KNMusicInfoCollectorManager::addAnalysisList(const QModelIndex &index,
                                                  const QString &filePath)
{
    m_indexList.append(index);
    m_fileList.append(filePath);
    if(!m_working)
    {
        m_working=true;
        emit requireAnalysis(m_fileList.at(0));
    }
}

void KNMusicInfoCollectorManager::currentSkip()
{
    m_indexList.removeFirst();
    m_fileList.removeFirst();
    if(m_fileList.size()==0)
    {
        m_working=false;
        emit requireSortData();
        return;
    }
    emit requireAnalysis(m_fileList.at(0));
}

void KNMusicInfoCollectorManager::currentWorkDone(const QStringList &value,
                                                  const KNMusicGlobal::MusicDetailsInfo &datas)
{
    m_currentFileData=value;
    m_currentFileAppendData=datas;
    emit requireUpdateRowInfo(m_indexList.at(0));
    currentSkip();
}

KNMusicGlobal::MusicDetailsInfo KNMusicInfoCollectorManager::currentFileAppendData() const
{
    return m_currentFileAppendData;
}

QStringList KNMusicInfoCollectorManager::currentFileData() const
{
    return m_currentFileData;
}
