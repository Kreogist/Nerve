#include "knmusiclistview.h"

#include "knmusicviewer.h"

KNMusicViewer::KNMusicViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    retranslate();
    test[0]=new KNMusicListView(this);
    test[1]=new KNMusicListView(this);
    test[2]=new KNMusicListView(this);
    test[3]=new KNMusicListView(this);
    test[4]=new KNMusicListView(this);
    addCategory(QPixmap(":/Category/Resources/Category/01_musics.png"),
                m_categoryCaption[Songs],
                test[0]);
    addCategory(QPixmap(":/Category/Resources/Category/02_artists.png"),
                m_categoryCaption[Artists],
                test[1]);
    addCategory(QPixmap(":/Category/Resources/Category/03_ablums.png"),
                m_categoryCaption[Albums],
                test[2]);
    addCategory(QPixmap(":/Category/Resources/Category/04_genres.png"),
                m_categoryCaption[Genres],
                test[3]);
    addCategory(QPixmap(":/Category/Resources/Category/05_playlists.png"),
                m_categoryCaption[Playlists],
                test[4]);
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
