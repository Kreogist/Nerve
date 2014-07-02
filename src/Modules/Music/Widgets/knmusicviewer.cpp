#include <QEvent>
#include <QKeyEvent>
#include <QModelIndex>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPropertyAnimation>

#include <QDebug>

#include "../Base/knmusicvieweritembase.h"
#include "../Base/knmusicviewerplaylistitembase.h"
#include "../../Base/knlibsearcher.h"
#include "../Libraries/knmusiclibrarymodelbase.h"

#include "../Plugins/knmusiclistviewitem.h"
#include "../Plugins/knmusicartistviewitem.h"
#include "../Plugins/knmusicalbumviewitem.h"
#include "../Plugins/knmusicgenreviewitem.h"
#include "../Plugins/knmusicplaylistviewitem.h"

#include "knmusicviewer.h"

KNMusicViewer::KNMusicViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    //Initial the translations.
    retranslate();

    //Set the properties.
//    setAcceptDrops(true);
    setContentsMargins(0,0,0,0);

    //Set the player widget animations.
    m_playerIn=new QPropertyAnimation(this);
    m_playerIn->setPropertyName("geometry");
    m_playerIn->setEasingCurve(QEasingCurve::OutCubic);

    m_playerOut=new QPropertyAnimation(this);
    m_playerOut->setPropertyName("geometry");
    m_playerOut->setEasingCurve(QEasingCurve::OutCubic);

    //Add plugins
    addListViewPlugin(new KNMusicListViewItem);
    addArtistViewPlugin(new KNMusicArtistViewItem);
    addAlbumViewPlugin(new KNMusicAlbumViewItem);
    addGenreViewPlugin(new KNMusicGenreViewItem);
    addPlaylistPlugin(new KNMusicPlaylistViewItem);
}

void KNMusicViewer::setMusicModel(KNMusicLibraryModelBase *model)
{
    emit requireSetMusicModel(model);
    m_musicModel=model;
}

bool KNMusicViewer::eventFilter(QObject *watched, QEvent *event)
{
    /*if(event->type()==QEvent::KeyRelease)
    {
        QKeyEvent *releasedKey=static_cast<QKeyEvent *>(event);
        if(releasedKey->modifiers()==Qt::ControlModifier)
        {
            switch(releasedKey->key())
            {
            case Qt::Key_Left:
                moveLeft();
                return true;
            case Qt::Key_Right:
                moveRight();
                return true;
            default:
                return KNStdLibViewer::eventFilter(watched, event);
            }
        }
    }*/
    if(event->type()==QEvent::MouseButtonRelease)
    {
        QMouseEvent *releaseMouse=static_cast<QMouseEvent *>(event);
        //if(releaseMouse)
        switch(releaseMouse->button())
        {
        case Qt::BackButton:
            moveLeft();
            return true;
        case Qt::ForwardButton:
            moveRight();
            return true;
        default:
            return KNStdLibViewer::eventFilter(watched, event);
        }
    }
    return KNStdLibViewer::eventFilter(watched, event);
}

void KNMusicViewer::setPlayWidget(QWidget *widget)
{
    m_playerWidget=widget;
    m_playerWidget->setGeometry(QRect(0, -height(), width(), height()));
    m_playerIn->setTargetObject(m_playerWidget);
    m_playerOut->setTargetObject(m_playerWidget);
}

void KNMusicViewer::addListViewPlugin(KNMusicViewerItemBase *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveOriginal,
            plugin, &KNMusicViewerItemBase::onActionRemoveOriginalItem);
    connect(this, &KNMusicViewer::requireShowInSongsView,
            plugin, &KNMusicViewerItemBase::onActionShowIndex);
    addDatabasePlugin(plugin);
}

void KNMusicViewer::addArtistViewPlugin(KNMusicViewerItemBase *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveMusic,
            plugin, &KNMusicViewerItemBase::onActionRemoveItem);
    connect(this, &KNMusicViewer::requireShowInArtistView,
            plugin, &KNMusicViewerItemBase::onActionShowIndex);
    addDatabasePlugin(plugin);
}

void KNMusicViewer::addAlbumViewPlugin(KNMusicViewerItemBase *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveMusic,
            plugin, &KNMusicViewerItemBase::onActionRemoveItem);
    connect(this, &KNMusicViewer::requireShowInAlbumView,
            plugin, &KNMusicViewerItemBase::onActionShowIndex);
    addDatabasePlugin(plugin);
}

void KNMusicViewer::addGenreViewPlugin(KNMusicViewerItemBase *plugin)
{
    connect(this, &KNMusicViewer::requireRemoveMusic,
            plugin, &KNMusicViewerItemBase::onActionRemoveItem);
    connect(this, &KNMusicViewer::requireShowInGenreView,
            plugin, &KNMusicViewerItemBase::onActionShowIndex);
    addDatabasePlugin(plugin);
}

void KNMusicViewer::addPlaylistPlugin(KNMusicViewerPlaylistItemBase *plugin)
{
    //Connect actions
    connect(this, &KNMusicViewer::requireSetPlaylistManager,
            plugin, &KNMusicViewerPlaylistItemBase::setPlaylistManager);

    //Connect requires
    connect(plugin, &KNMusicViewerPlaylistItemBase::requireAddCategory,
            this, &KNMusicViewer::addCategory);
    connect(plugin, &KNMusicViewerPlaylistItemBase::requirePlayMusic,
            this, &KNMusicViewer::requirePlayMusic);
    plugin->applyPlugin();
}

void KNMusicViewer::retranslate()
{
    ;
}

void KNMusicViewer::retranslateAndSet()
{
    retranslate();
}

void KNMusicViewer::showIn(const int &category, const QModelIndex &index)
{
    emit requireClearSearch();
    switch(category)
    {
    case KNMusicGlobal::SongsView:
        emit requireShowInSongsView(index);
        break;
    case KNMusicGlobal::ArtistView:
        emit requireShowInArtistView(index);
        break;
    case KNMusicGlobal::AlbumView:
        emit requireShowInAlbumView(index);
        break;
    case KNMusicGlobal::GenreView:
        emit requireShowInGenreView(index);
        break;
    }
    setCategoryIndex(category);
}

void KNMusicViewer::showInCurrent(const QModelIndex &index)
{
    showIn(categoryIndex(), index);
}

void KNMusicViewer::deleteMusic(const QModelIndex &index)
{
    emit requireRemoveMusic(index);
    m_musicModel->removeAppendData(index);
    emit requireRemoveOriginal(index);
}

void KNMusicViewer::deleteSelections()
{
    QModelIndexList selectionList=KNMusicGlobal::instance()->selectedIndexes();
    QList<int> rowLists;
    while(!selectionList.isEmpty())
    {
        rowLists.append(selectionList.takeFirst().row());
    }
    qStableSort(rowLists.begin(), rowLists.end(), qGreater<int>());
    while(!rowLists.isEmpty())
    {
        deleteMusic(m_musicModel->index(rowLists.takeFirst(), 0));
    }
}

void KNMusicViewer::onActionShowPlayer()
{
    m_playerOut->stop();
    if(m_playerIn->state()==QPropertyAnimation::Running)
    {
        m_playerIn->pause();
    }
    m_playerIn->setStartValue(m_playerWidget->geometry());
    m_playerIn->setEndValue(geometry());
    m_playerIn->start();
}

void KNMusicViewer::onActionHidePlayer()
{
    m_playerIn->stop();
    if(m_playerOut->state()==QPropertyAnimation::Running)
    {
        m_playerOut->pause();
    }
    m_playerOut->setStartValue(m_playerWidget->geometry());
    m_playerOut->setEndValue(QRect(0, -height(), width(), height()));
    m_playerOut->start();
}

void KNMusicViewer::resizeEvent(QResizeEvent *event)
{
    KNStdLibViewer::resizeEvent(event);
    m_playerWidget->setGeometry(m_playerWidget->y()==0?
                            geometry():QRect(0, -height(), width(), height()));
}

void KNMusicViewer::addDatabasePlugin(KNMusicViewerItemBase *plugin)
{
    //Connect actions.
    connect(this, &KNMusicViewer::requireResort,
            plugin, &KNMusicViewerItemBase::onActionResort);
    connect(this, &KNMusicViewer::requireSearch,
            plugin, &KNMusicViewerItemBase::onActionSearch);
    connect(this, &KNMusicViewer::requireSetMusicModel,
            plugin, &KNMusicViewerItemBase::setMusicSourceModel);
    connect(this, &KNMusicViewer::requireSetBackend,
            plugin, &KNMusicViewerItemBase::setBackend);

    //Connect requires.
    connect(plugin, &KNMusicViewerItemBase::requirePlayMusic,
            [=](const QModelIndex &sourceIndex)
            {
                emit requirePlayMusic(m_musicModel->filePathFromIndex(sourceIndex));
            });
    connect(plugin, &KNMusicViewerItemBase::requireShowContextMenu,
            this, &KNMusicViewer::requireShowContextMenu);
    connect(plugin, &KNMusicViewerItemBase::requireSetProxy,
            this, &KNMusicViewer::requireSetProxy);
    connect(plugin, &KNMusicViewerItemBase::requireAddCategory,
            this, &KNMusicViewer::addCategory);
    connect(plugin, &KNMusicViewerItemBase::requireAnalysisUrls,
            this, &KNMusicViewer::requireAnalysisUrls);

    //Apply plugin.
    plugin->applyPlugin();
}
