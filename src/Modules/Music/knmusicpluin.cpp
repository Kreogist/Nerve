#include "../knglobal.h"

#include <QList>
#include <QUrl>

#include "Libraries/knmusicmodel.h"
#include "Libraries/knmusicinfocollector.h"
#include "Libraries/knmusicinfocollectormanager.h"
#include "Libraries/knmusicsearcher.h"
#include "Widgets/knmusicviewer.h"

#include "knmusicpluin.h"

KNMusicPluin::KNMusicPluin(QObject *parent) :
    KNPluginBase(parent)
{
    m_global=KNGlobal::instance();
    m_model=new KNMusicModel(this);

    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
    m_musicViewer->setModel(m_model);

    m_searcher=new KNMusicSearcher;
    connect(m_musicViewer, &KNMusicViewer::requireAnalysisUrls,
            m_searcher, &KNMusicSearcher::analysisList);

    m_infoCollectManager=new KNMusicInfoCollectorManager(this);
    connect(m_searcher, &KNMusicSearcher::requireAnalysis,
            m_infoCollectManager, &KNMusicInfoCollectorManager::addAnalysisList);
    connect(m_infoCollectManager, &KNMusicInfoCollectorManager::requireAppendMusic,
            m_model, &KNMusicModel::appendMusic);

    /*m_infoCollector=new KNMusicInfoCollector(this);
    connect(m_searcher, &KNMusicSearcher::requireAnalysis,
            m_infoCollector, &KNMusicInfoCollector::analysis);
    connect(m_infoCollector, &KNMusicInfoCollector::requireAppendMusic,
            m_model, &KNMusicModel::appendMusic);*/
}

KNMusicPluin::~KNMusicPluin()
{
    m_searcher->deleteLater();
}

void KNMusicPluin::applyPlugin()
{
    emit requireAddCategory("test",
                            QPixmap(),
                            m_musicViewer);
}
