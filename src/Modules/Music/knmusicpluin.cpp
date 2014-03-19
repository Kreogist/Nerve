#include "../knglobal.h"

#include "Libraries/knmusicmodel.h"
#include "Libraries/knmusicinfocollector.h"
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

    m_searcher=new KNMusicSearcher(this);
    connect(m_musicViewer, SIGNAL(requireAnalysisUrls(QList<QUrl>)),
            m_searcher, SLOT(analysisList(QList<QUrl>)));

    m_infoCollector=new KNMusicInfoCollector(this);
    connect(m_searcher, &KNMusicSearcher::requireAnalysis,
            m_infoCollector, &KNMusicInfoCollector::analysis);
    connect(m_infoCollector, &KNMusicInfoCollector::requireAppendMusic,
            m_model, &KNMusicModel::appendMusic);
}

void KNMusicPluin::applyPlugin()
{
    emit requireAddCategory("test",
                            QPixmap(),
                            m_musicViewer);
}
