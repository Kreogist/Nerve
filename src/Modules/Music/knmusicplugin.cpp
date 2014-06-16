#include "../knglobal.h"

#include <QAction>
#include <QKeySequence>
#include <QList>
#include <QFile>
#include <QDir>
#include <QSortFilterProxyModel>

#include <QDebug>

#include "Libraries/knmusicmodel.h"
#include "Libraries/knmusicinfocollector.h"
#include "Libraries/knmusicinfocollectormanager.h"
#include "Libraries/knmusicfilter.h"
#include "Libraries/knmusicdatabase.h"
#include "Libraries/knmusicplaylistmanager.h"
#include "Libraries/knmusicbackend.h"
#include "Widgets/knmusicdetailinfo.h"
#include "Widgets/knmusicheaderwidget.h"
#include "Widgets/knmusicplayerwidget.h"
#include "Widgets/knmusicviewer.h"
#include "Widgets/knmusicviewermenu.h"
#include "Widgets/knmusiceq.h"

#include "knmusicplugin.h"

KNMusicPlugin::KNMusicPlugin(QObject *parent) :
    KNPluginBase(parent)
{
    //Initial global instance and file pathes.
    m_global=KNGlobal::instance();
    m_musicAlbumArt=QDir::toNativeSeparators(m_global->databaseFolder()+"/AlbumArt/");
    m_musicDatabase=QDir::toNativeSeparators(m_global->databaseFolder()+"/Music.db");

    //Initial music backend.
    m_musicPlayer=new KNMusicBackend;
    m_musicPlayer->moveToThread(&m_playerThread);

    //Initial music model
    m_model=new KNMusicModel;
    m_model->moveToThread(&m_modelThread);
    m_model->setAlbumArtPath(m_musicAlbumArt);
    connect(this, &KNMusicPlugin::requireAddRawFiles,
            m_model, &KNMusicModel::addRawFileItems);

    //Initial music data base for storage.
    m_database=new KNMusicDatabase;
    m_database->moveToThread(&m_databaseThread);
    m_database->setDatabase(m_musicDatabase);
    m_database->setModel(m_model);

    //Initial playlist manager.
    m_playlistManager=new KNMusicPlaylistManager(this);
    m_playlistManager->setMusicBackend(m_musicPlayer->backend());

    //Initial music viewer.
    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
    m_musicViewer->setPlaylistManager(m_playlistManager);
    m_musicViewer->setModel(m_model);
    m_musicViewer->setMusicBackend(m_musicPlayer->backend());
    connect(m_musicViewer, &KNMusicViewer::requireAnalysisUrls,
            this, &KNMusicPlugin::requireAnalysisUrls);
    connect(m_model, &KNMusicModel::requireResort,
            m_musicViewer, &KNMusicViewer::resort);

    //Initial header widget
    m_headerWidget=new KNMusicHeaderWidget(m_global->mainWindow());
    m_headerWidget->setPlaylistManager(m_playlistManager); //This must be done first!
    m_headerWidget->setMusicModel(m_model);
    m_headerWidget->setBackend(m_musicPlayer);
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
            m_headerWidget, &KNMusicHeaderWidget::setProxyModel);
    connect(m_musicViewer, &KNMusicViewer::requirePlayMusic,
            m_headerWidget, &KNMusicHeaderWidget::onActionPlayInLibrary);

    m_libraryViewMenu=new KNMusicViewerMenu(m_musicViewer);
    m_libraryViewMenu->setModel(m_model);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requirePlayMusic,
            m_headerWidget, &KNMusicHeaderWidget::onActionPlayInLibrary);
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

    m_searcher=new KNMusicFilter;
    m_searcher->moveToThread(&m_searcherThread);
    setFileFilter(m_searcher);

    m_infoCollectManager=new KNMusicInfoCollectorManager;
    m_infoCollectManager->moveToThread(&m_collectThread);
    m_infoCollectManager->setMusicBackend(m_musicPlayer->backend());
    m_model->setInfoCollectorManager(m_infoCollectManager);

    m_musicPlayerWidget=new KNMusicPlayerWidget(m_musicViewer);
    m_musicPlayerWidget->setHeaderPlayer(m_headerWidget->player());
    m_musicPlayerWidget->setBackend(m_musicPlayer);
    connect(m_headerWidget, &KNMusicHeaderWidget::requireUpdatePlaylistModel,
            m_musicPlayerWidget, &KNMusicPlayerWidget::setPlayListModel);

    m_equalizer=new KNMusicEQ(m_musicPlayer->backend());
    m_musicPlayerWidget->setEqualizer(m_equalizer);
    m_musicViewer->setPlayWidget(m_musicPlayerWidget);

    m_detailsDialog=new KNMusicDetailInfo(m_musicViewer);

    createShortcuts();

    m_modelThread.start();
    m_collectThread.start();
    m_databaseThread.start();
    m_searcherThread.start();
    m_playerThread.start();

    m_database->load();
    m_playlistManager->loadPlayLists();
}

KNMusicPlugin::~KNMusicPlugin()
{
    m_database->flush();

    m_collectThread.quit();
    m_playerThread.quit();
    m_searcherThread.quit();
    m_modelThread.quit();
    m_databaseThread.quit();

    m_collectThread.wait();
    m_playerThread.wait();
    m_searcherThread.wait();
    m_modelThread.wait();
    m_databaseThread.wait();

    m_model->deleteLater();
    m_musicPlayer->deleteLater();
    m_searcher->deleteLater();
    m_infoCollectManager->deleteLater();
    m_database->deleteLater();
}

void KNMusicPlugin::applyPlugin()
{
    emit requireAddCategory("Music",
                            QPixmap(":/Music/Resources/Music/icon.png"),
                            m_musicViewer);
    emit requireAddHeader(m_headerWidget);
}

void KNMusicPlugin::setFileFilter(KNLibFilter *filter)
{
    connect(this, &KNMusicPlugin::requireAnalysisUrls,
            filter, &KNLibFilter::analysisUrls);
    connect(filter, &KNLibFilter::requireAddRawFiles,
            this, &KNMusicPlugin::requireAddRawFiles);
}

void KNMusicPlugin::onActionShowContextMenu(const QPoint &position,
                                    KNMusicGlobal::MusicCategory currentMode)
{
    m_libraryViewMenu->setMode(currentMode);
    m_libraryViewMenu->readIndexesFromGlobal();
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
    QAction *musicPlay=new QAction(this);
    musicPlay->setShortcut(QKeySequence(Qt::Key_MediaPlay));
    connect(musicPlay, &QAction::triggered,
            [=]
            {
                m_musicPlayer->play();
            });
    m_musicViewer->addAction(musicPlay);
    m_headerWidget->addAction(musicPlay);
    QAction *musicPause=new QAction(this);
    musicPause->setShortcut(QKeySequence(Qt::Key_MediaPause));
    connect(musicPause, &QAction::triggered,
            [=]
            {
                m_musicPlayer->pause();
            });
    m_musicViewer->addAction(musicPause);
    m_headerWidget->addAction(musicPause);
}
