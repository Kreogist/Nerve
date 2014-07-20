#include <QBoxLayout>
#include <QResizeEvent>
#include <QFileInfo>

#include <QDebug>

#include "../../Base/knsearchbox.h"
#include "../Libraries/knmusiclibrarymodel.h"
#include "../Base/knmusicplaylistmanagerbase.h"
#include "../Libraries/knmusicinfocollector.h"
#include "../knmusicglobal.h"
#include "knmusicheaderplayer.h"
#include "knmusiclrcviewer.h"

#include "knmusicheaderwidget.h"

KNMusicHeaderWidget::KNMusicHeaderWidget(QWidget *parent) :
    KNStdLibHeaderWidget(parent)
{
    //Set properties.
    setContentsMargins(0,0,0,0);

    //Initial layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setContentsMargins(0,0,10,0);
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
    m_mainLayout->addWidget(m_headerPlayer, 0, Qt::AlignLeft | Qt::AlignVCenter);

    m_lrcViewer=new KNMusicLRCViewer(this);
    m_mainLayout->addWidget(m_lrcViewer, 1);

    m_searchBox=new KNSearchBox(this);
    m_searchBox->setFixedWidth(200);
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
    connect(m_searchBox, &KNSearchBox::textEdited,
            this, &KNMusicHeaderWidget::requireSearch);
    connect(m_searchBox, &KNSearchBox::requireLostFocus,
            this, &KNMusicHeaderWidget::requireLostFocus);
    m_mainLayout->addWidget(m_searchBox, 0, Qt::AlignRight | Qt::AlignVCenter);

    m_infoCollector=new KNMusicInfoCollector(this);
    m_infoCollector->setSignalMode(false);
}

void KNMusicHeaderWidget::setPlaylistManager(KNMusicPlaylistManagerBase *manager)
{
    m_playlistManager=manager;
    connect(m_playlistManager, &KNMusicPlaylistManagerBase::requireUpdatePlaylistModel,
            this, &KNMusicHeaderWidget::requireUpdatePlaylistModel);
    connect(m_headerPlayer, &KNMusicHeaderPlayer::requireChangeLoop,
            m_playlistManager, &KNMusicPlaylistManagerBase::setLoopMode);
    connect(m_playlistManager, &KNMusicPlaylistManagerBase::requireResetPlayer,
            this, &KNMusicHeaderWidget::resetPlayer);
    connect(m_playlistManager, &KNMusicPlaylistManagerBase::requirePlayFile,
            this, &KNMusicHeaderWidget::onActionPlayMusic);
}

void KNMusicHeaderWidget::setMusicModel(KNMusicLibraryModelBase *model)
{
    m_musicModel=model;
    m_playlistManager->setMusicModel(model);
}

void KNMusicHeaderWidget::setBackend(KNMusicBackend *backend)
{
    m_headerPlayer->setMusicBackend(backend);
    m_infoCollector->setMusicBackend(backend);
    m_lrcViewer->setMusicBackend(backend);
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
    emit requireSearch("");
}

void KNMusicHeaderWidget::onActionPlayMusic(const QString &filePath)
{
//    m_currentPath=filePath;
//    m_playlistManager->setCurrentPlaying(m_currentPath);
    //    QModelIndex currentIndex=m_musicModel->indexFromFilePath(m_currentPath);
    //    if(currentIndex.isValid())
    //    {
    //        m_headerPlayer->setAlbumArt(QPixmap::fromImage(m_musicModel->artwork(currentIndex.row())));
    //        m_headerPlayer->setTitle(m_musicModel->itemText(currentIndex.row(), KNMusicGlobal::Name));
    //        m_headerPlayer->setArtist(m_musicModel->itemText(currentIndex.row(), KNMusicGlobal::Artist));
    //        m_headerPlayer->setAlbum(m_musicModel->itemText(currentIndex.row(), KNMusicGlobal::Album));
    //    }
    //    else
    //    {
    //        m_infoCollector->analysis(m_currentPath);
    //        QStringList values=m_infoCollector->currentMusicValue();
    //        KNMusicGlobal::MusicDetailsInfo details=m_infoCollector->currentMusicDatas();
    //        m_headerPlayer->setAlbumArt(QPixmap::fromImage(details.coverImage));
    //        m_headerPlayer->setTitle(values.at(KNMusicGlobal::Name));
    //        m_headerPlayer->setArtist(values.at(KNMusicGlobal::Artist));
    //        m_headerPlayer->setAlbum(values.at(KNMusicGlobal::Album));
    //    }
        m_headerPlayer->syncData();
    //    loadCurrentLyrics();
        m_headerPlayer->playFile(filePath);
}

void KNMusicHeaderWidget::onActionPlayListPrev()
{
//    if(m_headerPlayer->position()>3)
//    {
//        m_headerPlayer->stop();
//        m_headerPlayer->play();
//        return;
//    }
//    QString pathTest=m_playlistManager->prevSong();
//    if(pathTest.isEmpty())
//    {
//        resetPlayer();
//    }
//    else
//    {
//        m_currentPath=pathTest;
//        playCurrent();
//    }
}

void KNMusicHeaderWidget::onActionPlayListNext()
{
//    QString pathTest=m_playlistManager->nextSong();
//    if(pathTest.isEmpty())
//    {
//        resetPlayer();
//    }
//    else
//    {
//        m_currentPath=pathTest;
//        playCurrent();
//    }
}

void KNMusicHeaderWidget::onActionCurrentFinished()
{
//    QString pathTest=m_playlistManager->nextPlayingSong();
//    if(pathTest.isEmpty())
//    {
//        resetPlayer();
//    }
//    else
//    {
//        m_currentPath=pathTest;
//        playCurrent();
//    }
}

void KNMusicHeaderWidget::loadCurrentLyrics()
{
//    QFileInfo currentFile(m_currentPath);
//    m_lrcViewer->loadLyrics(currentFile.absolutePath()+"/"+
//                            currentFile.completeBaseName()+"."+
//                            "lrc");
}

void KNMusicHeaderWidget::resetPlayer()
{
    m_headerPlayer->setAlbumArt(QPixmap());
    m_headerPlayer->setTitle("");
    m_headerPlayer->setArtist("");
    m_headerPlayer->setAlbum("");
    m_headerPlayer->stop();
}
