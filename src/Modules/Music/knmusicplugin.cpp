#include <QAction>
#include <QKeySequence>
#include <QFile>
#include <QDir>
#include <QSortFilterProxyModel>

#include <QDebug>

//Abstract class.
#include "../Base/knlibsearcher.h"
#include "../Public/Base/knmusicbackend.h"
#include "Libraries/knmusicdatabasebase.h"
#include "Libraries/knmusiclibrarymodelbase.h"

//Differentiation class.
#include "Libraries/knmusicsearcher.h"
#include "Libraries/knmusicdatabase.h"
#include "../Public/bass/knlibbass.h"

#include "Libraries/knmusiclibrarymodel.h"
#include "Libraries/knmusicinfocollectormanager.h"
#include "Libraries/knmusicplaylistmanager.h"
#include "Widgets/knmusicdetailinfo.h"
#include "Widgets/knmusicheaderwidget.h"
#include "Widgets/knmusicplayerwidget.h"
#include "Widgets/knmusicviewer.h"
#include "Widgets/knmusicviewermenu.h"
#include "Widgets/knmusiceq.h"

#include "../knglobal.h"

#include "knmusicplugin.h"

KNMusicPlugin::KNMusicPlugin(QObject *parent) :
    KNPluginBase(parent)
{
    //Initial global instance and file pathes.
    m_global=KNGlobal::instance();
    m_musicAlbumArt=QDir::toNativeSeparators(m_global->databaseFolder()+"/AlbumArt/");
    m_musicDatabasePath=QDir::toNativeSeparators(m_global->databaseFolder()+"/Music.db");

    //Initial music backend.
    setBackend(new KNLibBass);

    //Initial music model
    m_libraryModel=new KNMusicLibraryModel;
    m_libraryModel->moveToThread(&m_modelThread);
    m_libraryModel->setAlbumArtPath(m_musicAlbumArt);
    connect(this, &KNMusicPlugin::requireAddRawFiles,
            m_libraryModel, &KNMusicLibraryModelBase::addRawFileItems);

    //Initial music data base for storage.
    setDatabase(new KNMusicDatabase);

    //Initial playlist manager.
    m_playlistManager=new KNMusicPlaylistManager(this);
    m_playlistManager->setBackend(m_backend);

    //Initial music viewer.
    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
    m_musicViewer->setPlaylistManager(m_playlistManager);
    m_musicViewer->setMusicModel(m_libraryModel);
    m_musicViewer->setBackend(m_backend);
    connect(m_musicViewer, &KNMusicViewer::requireAnalysisUrls,
            this, &KNMusicPlugin::requireAnalysisUrls);
    connect(m_libraryModel, &KNMusicLibraryModelBase::requireResort,
            m_musicViewer, &KNMusicViewer::resort);

    //Initial header widget
    m_headerWidget=new KNMusicHeaderWidget(m_global->mainWindow());
    m_headerWidget->setPlaylistManager(m_playlistManager); //This must be done first!
    m_headerWidget->setMusicModel(m_libraryModel);
    m_headerWidget->setBackend(m_backend);
    connect(m_headerWidget, &KNMusicHeaderWidget::requireSearch,
            m_musicViewer, &KNMusicViewer::onActionSearch);
    connect(m_headerWidget, &KNMusicHeaderWidget::requireShowMusicPlayer,
            m_musicViewer, &KNMusicViewer::onActionShowPlayer);
    connect(m_headerWidget, &KNMusicHeaderWidget::requireHideMusicPlayer,
            m_musicViewer, &KNMusicViewer::onActionHidePlayer);
    connect(m_headerWidget, &KNMusicHeaderWidget::requireLostFocus,
            m_musicViewer, &KNMusicViewer::setContentsFocus);
    connect(m_musicViewer, &KNMusicViewer::requireClearSearch,
            m_headerWidget, &KNMusicHeaderWidget::clearSearch);
    connect(m_musicViewer, &KNMusicViewer::requireSetProxy,
            m_playlistManager, &KNMusicPlaylistManager::setProxyModel);
    connect(m_musicViewer, &KNMusicViewer::requirePlayMusic,
            m_headerWidget, &KNMusicHeaderWidget::onActionPlayMusic);

    m_libraryViewMenu=new KNMusicViewerMenu(m_musicViewer);
    m_libraryViewMenu->setModel(m_libraryModel);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requirePlayMusic,
            m_headerWidget, &KNMusicHeaderWidget::onActionPlayMusic);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireShowIn,
            m_musicViewer, &KNMusicViewer::showIn);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireGetInfo,
            this, &KNMusicPlugin::onActionGetInfo);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireDelete,
            m_musicViewer, &KNMusicViewer::deleteMusic);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireDeleteSelection,
            m_musicViewer, &KNMusicViewer::deleteSelections);
    connect(m_musicViewer, &KNMusicViewer::requireShowContextMenu,
            this, &KNMusicPlugin::onActionShowContextMenu);

    setSearcher(new KNMusicSearcher);

    m_infoCollectManager=new KNMusicInfoCollectorManager;
    m_infoCollectManager->moveToThread(&m_collectThread);
    m_infoCollectManager->setMusicBackend(m_backend);
    m_libraryModel->setInfoCollectorManager(m_infoCollectManager);

    m_musicPlayerWidget=new KNMusicPlayerWidget(m_musicViewer);
    m_musicPlayerWidget->setHeaderPlayer(m_headerWidget->player());
    m_musicPlayerWidget->setBackend(m_backend);
    connect(m_headerWidget, &KNMusicHeaderWidget::requireUpdatePlaylistModel,
            m_musicPlayerWidget, &KNMusicPlayerWidget::setPlayListModel);

    m_equalizer=new KNMusicEQ(m_backend);
    m_musicPlayerWidget->setEqualizer(m_equalizer);
    m_musicViewer->setPlayWidget(m_musicPlayerWidget);

    setDetailsDialog(new KNMusicDetailInfo(m_musicViewer));

    loadShortcuts();
    loadThreads();
    loadData();
}

KNMusicPlugin::~KNMusicPlugin()
{
    m_musicDatabase->flush();

    m_collectThread.quit();
    m_backendThread.quit();
    m_searcherThread.quit();
    m_modelThread.quit();
    m_databaseThread.quit();

    m_collectThread.wait();
    m_backendThread.wait();
    m_searcherThread.wait();
    m_modelThread.wait();
    m_databaseThread.wait();

    m_libraryModel->deleteLater();
    m_backend->deleteLater();
    m_searcher->deleteLater();
    m_infoCollectManager->deleteLater();
    m_musicDatabase->deleteLater();
}

void KNMusicPlugin::applyPlugin()
{
    emit requireAddCategory("Music",
                            QPixmap(":/Music/Resources/Music/icon.png"),
                            m_musicViewer);
    emit requireAddHeader(m_headerWidget);
}

void KNMusicPlugin::setSearcher(KNLibSearcher *searcher)
{
    m_searcher=searcher;
    m_searcher->moveToThread(&m_searcherThread);
    connect(this, &KNMusicPlugin::requireAnalysisUrls,
            m_searcher, &KNLibSearcher::analysisUrls);
    connect(m_searcher, &KNLibSearcher::requireAddRawFiles,
            this, &KNMusicPlugin::requireAddRawFiles);
}

void KNMusicPlugin::setDatabase(KNMusicDatabaseBase *database)
{
    m_musicDatabase=database;
    m_musicDatabase->moveToThread(&m_databaseThread);
    m_musicDatabase->setDatabase(m_musicDatabasePath);
    m_musicDatabase->setModel(m_libraryModel);
}

void KNMusicPlugin::setDetailsDialog(KNMusicDetailInfoBase *detailInfoDialog)
{
    m_detailsDialog=detailInfoDialog;
}

void KNMusicPlugin::setBackend(KNMusicBackend *backend)
{
    m_backend=backend;
    m_backend->moveToThread(&m_backendThread);
}

void KNMusicPlugin::loadThreads()
{
    m_modelThread.start();
    m_collectThread.start();
    m_databaseThread.start();
    m_searcherThread.start();
    m_backendThread.start();
}

void KNMusicPlugin::loadData()
{
    m_musicDatabase->load();
    m_playlistManager->loadPlayLists();
}

void KNMusicPlugin::onActionShowContextMenu(const QPoint &position,
                                            int currentMode)
{
    m_libraryViewMenu->setMode(currentMode);
    m_libraryViewMenu->readIndexesFromGlobal();
    m_libraryViewMenu->exec(position);
}

void KNMusicPlugin::onActionOpenUrl(const QModelIndex &index)
{
    QString filePath=m_libraryModel->filePathFromIndex(index);
    m_libraryModel->updateIndexInfo(index, filePath);
    m_global->openLocalUrl(filePath);
}

void KNMusicPlugin::onActionGetInfo(const QString &filePath)
{
    m_detailsDialog->setFilePath(filePath);
    m_detailsDialog->show();
}

void KNMusicPlugin::loadShortcuts()
{
    QAction *searchShortcut=new QAction(m_musicViewer);
    searchShortcut->setShortcut(QKeySequence::Find);
    connect(searchShortcut, SIGNAL(triggered()),
            m_headerWidget, SLOT(setSearchFocus()));
    m_musicViewer->addAction(searchShortcut);
    QAction *musicPlay=new QAction(this);
    musicPlay->setShortcut(QKeySequence(Qt::Key_MediaPlay));
    connect(musicPlay, &QAction::triggered,
            [=]
            {
                m_backend->play();
            });
    m_musicViewer->addAction(musicPlay);
    m_headerWidget->addAction(musicPlay);
    QAction *musicPause=new QAction(this);
    musicPause->setShortcut(QKeySequence(Qt::Key_MediaPause));
    connect(musicPause, &QAction::triggered,
            [=]
            {
                m_backend->pause();
            });
    m_musicViewer->addAction(musicPause);
    m_headerWidget->addAction(musicPause);
}
