#include "../knglobal.h"

#include <QList>
#include <QFile>
#include <QDir>

#include <QDebug>

#include "Libraries/knmusicmodel.h"
#include "Libraries/knmusicinfocollector.h"
#include "Libraries/knmusicinfocollectormanager.h"
#include "Libraries/knmusicsearcher.h"
#include "Widgets/knmusicdetailinfo.h"
#include "Widgets/knmusicviewer.h"
#include "Widgets/knmusicviewermenu.h"

#include "knmusicplugin.h"

KNMusicPlugin::KNMusicPlugin(QObject *parent) :
    KNPluginBase(parent)
{
    m_global=KNGlobal::instance();
    m_model=new KNMusicModel;
    m_model->moveToThread(&m_modelThread);

    m_musicDatabase=QDir::toNativeSeparators(m_global->databaseFolder()+"/Music");

    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
    m_musicViewer->setModel(m_model);
    connect(m_musicViewer, SIGNAL(requireOpenUrl(QModelIndex)),
            this, SLOT(onActionOpenUrl(QModelIndex)));

    m_libraryViewMenu=new KNMusicViewerMenu(m_musicViewer);
    m_libraryViewMenu->setModel(m_model);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireShowIn,
            m_musicViewer, &KNMusicViewer::showIn);
    connect(m_libraryViewMenu, &KNMusicViewerMenu::requireGetInfo,
            this, &KNMusicPlugin::onActionGetInfo);
    connect(m_musicViewer, &KNMusicViewer::requireShowContextMenu,
            this, &KNMusicPlugin::onActionShowContextMenu);

    m_searcher=new KNMusicSearcher(this);
    m_searcher->setModel(m_model);
    m_musicViewer->setSearcher(m_searcher);
    connect(m_model, &KNMusicModel::requireResort,
            m_musicViewer, &KNMusicViewer::resort);

    m_infoCollectManager=new KNMusicInfoCollectorManager;
    m_infoCollectManager->moveToThread(&m_collectThread);
    m_model->setInfoCollectorManager(m_infoCollectManager);

    m_modelThread.start();
    m_collectThread.start();

    m_detailsDialog=new KNMusicDetailInfo(m_musicViewer);
}

KNMusicPlugin::~KNMusicPlugin()
{
    m_collectThread.quit();
    m_collectThread.wait();

    m_modelThread.quit();
    m_modelThread.wait();

    m_model->deleteLater();
    m_infoCollectManager->deleteLater();
}

void KNMusicPlugin::applyPlugin()
{
    emit requireAddCategory("Music",
                            QPixmap(),
                            m_musicViewer);
}

void KNMusicPlugin::writeDatabase()
{
    QFile musicDatabase(m_musicDatabase);
    if(musicDatabase.open(QIODevice::WriteOnly))
    {
        QDataStream dataOut(&musicDatabase);
        m_model->writeToDataStream(dataOut);
        musicDatabase.close();
    }
}

void KNMusicPlugin::readDatabase()
{
    QFile musicDatabase(m_musicDatabase);
    if(musicDatabase.exists() &&
            musicDatabase.open(QIODevice::ReadOnly))
    {
        QDataStream dataIn(&musicDatabase);
        m_model->readFromDataStream(dataIn);
        musicDatabase.close();
    }
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
