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
    m_model=new KNMusicModel;
    m_model->moveToThread(&m_modelThread);

    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
    m_musicViewer->setModel(m_model);

    m_searcher=new KNMusicSearcher(this);
    connect(m_musicViewer, &KNMusicViewer::requireAnalysisUrls,
            m_searcher, &KNMusicSearcher::analysisList);

    m_infoCollectManager=new KNMusicInfoCollectorManager;
    m_infoCollectManager->moveToThread(&m_collectThread);
    connect(m_searcher, &KNMusicSearcher::requireAnalysis,
            m_infoCollectManager, &KNMusicInfoCollectorManager::addAnalysisList);
    connect(m_infoCollectManager, &KNMusicInfoCollectorManager::requireAppendMusic,
            m_model, &KNMusicModel::appendMusic);

    m_modelThread.start();
    m_collectThread.start();
}

KNMusicPluin::~KNMusicPluin()
{
    m_collectThread.quit();
    m_collectThread.wait();

    m_modelThread.quit();
    m_modelThread.wait();

    m_model->deleteLater();
    m_infoCollectManager->deleteLater();
}

void KNMusicPluin::applyPlugin()
{
    emit requireAddCategory("test",
                            QPixmap(),
                            m_musicViewer);
}
