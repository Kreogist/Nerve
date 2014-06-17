#include <QBoxLayout>
#include <QResizeEvent>

#include <QDebug>

#include "../../Base/knsearchbox.h"
#include "../Libraries/knmusicmodel.h"
#include "../Libraries/knmusicplaylistmanager.h"
#include "../Libraries/knmusicinfocollector.h"
#include "../knmusicglobal.h"
#include "knmusicheaderplayer.h"

#include "knmusicheaderwidget.h"

KNMusicHeaderWidget::KNMusicHeaderWidget(QWidget *parent) :
    KNStdLibHeaderWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Initial layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setAlignment(Qt::AlignVCenter);
    setLayout(m_mainLayout);

    //Initial the header player widget.
    m_headerPlayer=new KNMusicHeaderPlayer(this);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireNext,
            this, &KNMusicHeaderWidget::onActionPlayListNext);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requirePrev,
            this, &KNMusicHeaderWidget::onActionPlayListPrev);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::finished,
            this, &KNMusicHeaderWidget::onActionCurrentFinished);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireShowMusicPlayer,
            [=]{
                    emit requireShowMusicPlayer();
               }
            );
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireHideMusicPlayer,
            this, &KNMusicHeaderWidget::requireHideMusicPlayer);
    m_mainLayout->addWidget(m_headerPlayer, 1, Qt::AlignLeft | Qt::AlignVCenter);

    m_searchBox=new KNSearchBox(this);
    m_searchBox->setFixedWidth(200);
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
    connect(m_searchBox, &KNSearchBox::textEdited,
            this, &KNMusicHeaderWidget::requireSearch);
    connect(m_searchBox, &KNSearchBox::requireLostFocus,
            this, &KNMusicHeaderWidget::requireLostFocus);
    m_mainLayout->addWidget(m_searchBox, 0, Qt::AlignRight | Qt::AlignVCenter);

    m_infoCollector=new KNMusicInfoCollector(this);
}

void KNMusicHeaderWidget::setPlaylistManager(KNMusicPlaylistManager *manager)
{
    m_playlistManager=manager;
    connect(m_playlistManager, &KNMusicPlaylistManager::requireUpdatePlaylistModel,
            this, &KNMusicHeaderWidget::requireUpdatePlaylistModel);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireChangeLoop,
            m_playlistManager, &KNMusicPlaylistManager::setLoopMode);
}

void KNMusicHeaderWidget::setMusicModel(KNMusicModel *model)
{
    m_musicModel=model;
    m_playlistManager->setMusicModel(model);
}

void KNMusicHeaderWidget::setProxyModel(QSortFilterProxyModel *model)
{
    m_playlistManager->setProxyModel(model);
}

void KNMusicHeaderWidget::setBackend(KNMusicBackend *backend)
{
    m_headerPlayer->setBackend(backend);
}

void KNMusicHeaderWidget::setAlbumArt(const QPixmap &albumArt)
{
    m_headerPlayer->setAlbumArt(albumArt);
}

void KNMusicHeaderWidget::setTitle(const QString &string)
{
    m_headerPlayer->setTitle(string);
}

void KNMusicHeaderWidget::setArtist(const QString &string)
{
    m_headerPlayer->setArtist(string);
}

void KNMusicHeaderWidget::setAlbum(const QString &string)
{
    m_headerPlayer->setAlbum(string);
}

KNMusicHeaderPlayer *KNMusicHeaderWidget::player()
{
    return m_headerPlayer;
}

void KNMusicHeaderWidget::retranslate()
{
    m_searchPlaceHolder=tr("Search Music");
}

void KNMusicHeaderWidget::retranslateAndSet()
{
    retranslate();
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
}

void KNMusicHeaderWidget::setSearchFocus()
{
    m_searchBox->setSearchFocus();
}

void KNMusicHeaderWidget::clearSearch()
{
    m_searchBox->clear();
}

void KNMusicHeaderWidget::onActionPlayMusic(const QString &filePath)
{
    m_currentPath=filePath;
    m_playlistManager->setCurrentPlaying(m_currentPath);
    playCurrent();
}

void KNMusicHeaderWidget::onActionPlayListPrev()
{
//    if(m_headerPlayer->position()>3)
//    {
//        m_headerPlayer->stop();
//        m_headerPlayer->play();
//        return;
//    }
    QString pathTest=m_playlistManager->prevSong();
    if(pathTest.isEmpty())
    {
        resetPlayer();
    }
    else
    {
        m_currentPath=pathTest;
        playCurrent();
    }
}

void KNMusicHeaderWidget::onActionPlayListNext()
{
    QString pathTest=m_playlistManager->nextSong();
    if(pathTest.isEmpty())
    {
        resetPlayer();
    }
    else
    {
        m_currentPath=pathTest;
        playCurrent();
    }
}

void KNMusicHeaderWidget::onActionCurrentFinished()
{
    QString pathTest=m_playlistManager->nextPlayingSong();
    if(pathTest.isEmpty())
    {
        resetPlayer();
    }
    else
    {
        m_currentPath=pathTest;
        playCurrent();
    }
}

void KNMusicHeaderWidget::playCurrent()
{
    QModelIndex currentIndex=m_musicModel->indexFromFilePath(m_currentPath);
    if(currentIndex.isValid())
    {
        m_headerPlayer->setAlbumArt(QPixmap::fromImage(m_musicModel->artwork(currentIndex.row())));
        m_headerPlayer->setTitle(m_musicModel->itemText(currentIndex.row(), KNMusicGlobal::Name));
        m_headerPlayer->setArtist(m_musicModel->itemText(currentIndex.row(), KNMusicGlobal::Artist));
        m_headerPlayer->setAlbum(m_musicModel->itemText(currentIndex.row(), KNMusicGlobal::Album));
        m_headerPlayer->syncData();
        m_headerPlayer->playFile(m_currentPath);
        return;
    }
    m_infoCollector->analysis(m_currentPath);
    m_headerPlayer->playFile(m_currentPath);
}

void KNMusicHeaderWidget::resetPlayer()
{
    m_headerPlayer->setAlbumArt(QPixmap());
    m_headerPlayer->setTitle("");
    m_headerPlayer->setArtist("");
    m_headerPlayer->setAlbum("");
    m_headerPlayer->stop();
}
