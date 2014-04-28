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
    m_collectThread.start();
}

KNMusicInfoCollectorManager::~KNMusicInfoCollectorManager()
{
    m_collectThread.quit();
    m_collectThread.wait();
    m_collector->deleteLater();
}

void KNMusicInfoCollectorManager::addAnalysisList(const QString &filePath)
{
    m_fileList.append(filePath);
    if(!m_working)
    {
        m_working=true;
        m_collector->analysis(m_fileList.at(0));
    }
}

void KNMusicInfoCollectorManager::currentSkip()
{
    m_fileList.removeFirst();
    if(m_fileList.count()==0)
    {
        m_working=false;
        emit requireSortData();
        return;
    }
    m_collector->analysis(m_fileList.at(0));
}

void KNMusicInfoCollectorManager::currentWorkDone(const QStringList &value,
                                                  const KNMusicGlobal::MusicDetailsInfo &datas)
{
    emit requireAppendMusic(value, datas);
    currentSkip();
}
