#include "../knglobal.h"

#include <QAction>
#include <QKeySequence>
#include <QList>
#include <QFile>
#include <QDir>

#include <QDebug>

#include "Libraries/knmusicmodel.h"
#include "Libraries/knmusicinfocollector.h"
#include "Libraries/knmusicinfocollectormanager.h"
#include "Libraries/knmusicfilter.h"
#include "Libraries/knmusicdatabase.h"
#include "Widgets/knmusicdetailinfo.h"
#include "Widgets/knmusicheaderwidget.h"
#include "Widgets/knmusicviewer.h"
#include "Widgets/knmusicviewermenu.h"

#include "knmusicplugin.h"

KNMusicPlugin::KNMusicPlugin(QObject *parent) :
    KNPluginBase(parent)
{
    m_global=KNGlobal::instance();
    m_musicAlbumArt=QDir::toNativeSeparators(m_global->databaseFolder()+"/AlbumArt/");
    m_model=new KNMusicModel;
    m_model->moveToThread(&m_modelThread);
    m_model->setAlbumArtPath(m_musicAlbumArt);

    m_musicDatabase=QDir::toNativeSeparators(m_global->databaseFolder()+"/Music.db");
    m_database=new KNMusicDatabase;
    m_database->moveToThread(&m_databaseThread);
    m_database->setDatabase(m_musicDatabase);
    m_database->setModel(m_model);

    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
    m_musicViewer->setModel(m_model);
    connect(m_musicViewer, &KNMusicViewer::requireOpenUrl,
            this, &KNMusicPlugin::onActionOpenUrl);

    m_headerWidget=new KNMusicHeaderWidget(m_global->mainWindow());
    connect(m_headerWidget, &KNMusicHeaderWidget::requireSearch,
            m_musicViewer, &KNMusicViewer::onActionSearch);
    connect(m_headerWidget, &KNMusicHeaderWidget::requireLostFocus,
            m_musicViewer, &KNMusicViewer::setContentsFocus);

    m_libraryViewMenu=new KNMusicViewerMenu(m_musicViewer);
    m_libraryViewMenu->setModel(m_model);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireShowIn,
            m_musicViewer, &KNMusicViewer::showIn);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireGetInfo,
            this, &KNMusicPlugin::onActionGetInfo);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireDelete,
            m_musicViewer, &KNMusicViewer::deleteMusic);
    connect(m_musicViewer, &KNMusicViewer::requireShowContextMenu,
            this, &KNMusicPlugin::onActionShowContextMenu);

    m_searcher=new KNMusicFilter;
    m_searcher->moveToThread(&m_searcherThread);
    m_searcher->setModel(m_model);
    m_musicViewer->setFilter(m_searcher);
    connect(m_model, &KNMusicModel::requireResort,
            m_musicViewer, &KNMusicViewer::resort);

    m_infoCollectManager=new KNMusicInfoCollectorManager;
    m_infoCollectManager->moveToThread(&m_collectThread);
    m_model->setInfoCollectorManager(m_infoCollectManager);

    m_detailsDialog=new KNMusicDetailInfo(m_musicViewer);

    createShortcuts();

    m_modelThread.start();
    m_collectThread.start();
    m_databaseThread.start();
    m_searcherThread.start();

    m_database->load();
}

KNMusicPlugin::~KNMusicPlugin()
{
    m_database->flush();

    m_collectThread.quit();
    m_searcherThread.quit();
    m_modelThread.quit();
    m_databaseThread.quit();

    m_collectThread.wait();
    m_searcherThread.wait();
    m_modelThread.wait();
    m_databaseThread.wait();

    m_model->deleteLater();
    m_searcher->deleteLater();
    m_infoCollectManager->deleteLater();
    m_database->deleteLater();
}

void KNMusicPlugin::applyPlugin()
{
    emit requireAddCategory("Music",
                            QPixmap(),
                            m_musicViewer);
    emit requireAddHeader(m_headerWidget);
}

void KNMusicPlugin::writeDatabase()
{
    ;
}

void KNMusicPlugin::readDatabase()
{
    ;
}

void KNMusicPlugin::onActionShowContextMenu(const QPoint &position,
                                    const QModelIndex &index,
                                    KNMusicGlobal::MusicCategory currentMode)
{
    m_libraryViewMenu->setMode(currentMode);
    m_libraryViewMenu->setItemIndex(index);
    m_libraryViewMenu->exec(position);
}

void KNMusicPlugin::onActionOpenUrl(const QModelIndex &index)
{
    QString filePath=m_model->filePathFromIndex(index);
    m_model->updateIndexInfo(index, filePath);
    m_global->openLocalUrl(filePath);
}

void KNMusicPlugin::onActionGetInfo(const QString &filePath)
{
    m_detailsDialog->setFilePath(filePath);
    m_detailsDialog->show();
}

void KNMusicPlugin::createShortcuts()
{
    QAction *searchShortcut=new QAction(m_musicViewer);
    searchShortcut->setShortcut(QKeySequence::Find);
    connect(searchShortcut, SIGNAL(triggered()),
            m_headerWidget, SLOT(setSearchFocus()));
    m_musicViewer->addAction(searchShortcut);
}
