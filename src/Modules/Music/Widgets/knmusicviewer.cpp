#include "knmusiclistview.h"

#include "../Libraries/knmusicmodel.h"
#include "../knmusicglobal.h"

#include "knmusicviewer.h"

KNMusicViewer::KNMusicViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    retranslate();
    setContentsMargins(0,0,0,0);

    m_libraryView=new KNMusicListView(this);

    QWidget *empty1=new QWidget(this),
            *empty2=new QWidget(this),
            *empty3=new QWidget(this),
            *empty4=new QWidget(this);

    addCategory(QPixmap(":/Category/Resources/Category/01_musics.png"),
                m_categoryCaption[Songs],
                m_libraryView);
    addCategory(QPixmap(":/Category/Resources/Category/02_artists.png"),
                m_categoryCaption[Artists],
                empty1);
    addCategory(QPixmap(":/Category/Resources/Category/03_ablums.png"),
                m_categoryCaption[Albums],
                empty2);
    addCategory(QPixmap(":/Category/Resources/Category/04_genres.png"),
                m_categoryCaption[Genres],
                empty3);
    addCategory(QPixmap(":/Category/Resources/Category/05_playlists.png"),
                m_categoryCaption[Playlists],
                empty4);
}

void KNMusicViewer::setModel(KNMusicModel *model)
{
    m_libraryView->setModel(model);
    m_libraryView->resetHeader();
}

void KNMusicViewer::retranslate()
{
    m_categoryCaption[Songs]=tr("Songs");
    m_categoryCaption[Artists]=tr("Artists");
    m_categoryCaption[Albums]=tr("Albums");
    m_categoryCaption[Genres]=tr("Genres");
    m_categoryCaption[Playlists]=tr("Playlists");
}

void KNMusicViewer::retranslateAndSet()
{
    retranslate();
}
