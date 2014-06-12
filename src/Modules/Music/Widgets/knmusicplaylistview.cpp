#include <QStringList>
#include <QFileDialog>
#include <QBoxLayout>

#include "../../Base/knverticalwidgetswitcher.h"
#include "../Libraries/knmusicplaylistmanager.h"
#include "knmusicfileexplorer.h"
#include "knmusicplaylistlistview.h"
#include "knmusicplaylistlisteditor.h"

#include "knmusicplaylistview.h"

KNMusicPlaylistView::KNMusicPlaylistView(QWidget *parent) :
    QSplitter(parent)
{
    //Get the latest translation.
    retranslate();

    //Set properties.
    setContentsMargins(0,0,0,0);
    setHandleWidth(0);

    //Initial playlist view.
    QWidget *categoryList=new QWidget(this);
    QBoxLayout *categoryLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                              categoryList);
    categoryLayout->setContentsMargins(0,0,0,0);
    categoryLayout->setSpacing(0);
    categoryList->setLayout(categoryLayout);
    m_categoryList=new KNMusicPlaylistListview(this);
    for(int i=0; i<HeaderCount; i++)
    {
        m_headerIndex[i]=m_categoryList->addHeader(m_headerCaption[i]);
    }
    categoryLayout->addWidget(m_categoryList, 1);
    m_listEditor=new KNMusicPlaylistListEditor(this);
    categoryLayout->addWidget(m_listEditor);
    connect(m_listEditor, &KNMusicPlaylistListEditor::requireCreatePlaylist,
            this, &KNMusicPlaylistView::onActionCreatePlaylist);
    connect(m_listEditor, &KNMusicPlaylistListEditor::requireOpen,
            this, &KNMusicPlaylistView::onActionOpenPlaylist);
    connect(m_listEditor, &KNMusicPlaylistListEditor::requireRemoveCurrent,
            this, &KNMusicPlaylistView::onActionRemoveCurrent);
    addWidget(categoryList);

    //Initial playlist displayer.
    KNVerticalWidgetSwitcher *viewer=new KNVerticalWidgetSwitcher(this);
    m_playlistViewer=new QWidget(this);
    m_dirViewer=new KNMusicFileExplorer(this);
    viewer->addWidget(m_dirViewer);
    viewer->addWidget(m_playlistViewer);
    addWidget(viewer);

    //Set properties after initial all the widgets.
    setCollapsible(1, false);
    setStretchFactor(1, 1);
}

void KNMusicPlaylistView::setPlaylistManager(KNMusicPlaylistManager *manager)
{
    m_playlistManager=manager;
    connect(m_playlistManager, &KNMusicPlaylistManager::playlistListUpdated,
            this, &KNMusicPlaylistView::onActionUpdatePlaylists);
    connect(m_categoryList, &KNMusicPlaylistListview::requireAddPlaylist,
            m_playlistManager, &KNMusicPlaylistManager::createPlaylist);
}

void KNMusicPlaylistView::retranslate()
{
    m_headerCaption[Playlist]=tr("Playlist");
    m_headerCaption[Dirs]=tr("Directory");
}

void KNMusicPlaylistView::retranslateAndSet()
{
    retranslate();
}

void KNMusicPlaylistView::onActionUpdatePlaylists()
{
    m_categoryList->setCurrentHeader(m_headerIndex[Playlist]);
    m_categoryList->clearHeader();
    QStringList playlistNames=m_playlistManager->playlistNameList();
    int playlistSize=playlistNames.size();
    while(playlistSize--)
    {
        m_categoryList->addItem(playlistNames.takeFirst());
    }
}

void KNMusicPlaylistView::onActionCreatePlaylist()
{
    //Set to playlist header.
    m_categoryList->setCurrentHeader(Playlist);
    //Add an item.
    m_categoryList->createItem();
}

void KNMusicPlaylistView::onActionOpenPlaylist()
{
    m_playlistManager->importPlaylist(
                QFileDialog::getOpenFileNames(this,
                                              tr("Open playlist"),
                                              "",
                                              tr("Nerve Playlist (*.json)")));
}

void KNMusicPlaylistView::onActionRemoveCurrent()
{
    //Should ask the user first.
    switch(m_categoryList->currentHeader())
    {
    case Playlist:
        m_playlistManager->removePlaylist(m_categoryList->currentItem());
        break;
    case Dirs:
        break;
    default:
        //Here should never comes.
        return;
    }
    m_categoryList->removeCurrentItem();
}
