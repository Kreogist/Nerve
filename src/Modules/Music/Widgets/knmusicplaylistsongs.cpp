#include <QTimeLine>
#include <QMap>
#include <QTableWidget>

#include "../knmusicglobal.h"

#include <QDebug>

#include "knmusicplaylistheader.h"
#include "knmusicplaylistindex.h"

#include "knmusicplaylistsongs.h"

KNMusicPlaylistSongs::KNMusicPlaylistSongs(QWidget *parent) :
    KNMusicListViewBase(parent)
{
    //Set properties.
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSortingEnabled(false);

    //Set palette.
    int minGrey=0x10;
    m_backgroundColor=QColor(minGrey, minGrey, minGrey);
    m_palette=palette();
    m_palette.setColor(QPalette::Base, m_backgroundColor);
    m_palette.setColor(QPalette::AlternateBase, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Window, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Button, QColor(0x20, 0x20, 0x20));
    m_palette.setColor(QPalette::Text, QColor(0x9f, 0x9f, 0x9f, 0xCF));
    m_palette.setColor(QPalette::Highlight, QColor(0x60, 0x60, 0x60));
    m_palette.setColor(QPalette::HighlightedText, QColor(0xf7, 0xcf, 0x3d));
    setPalette(m_palette);

    //Set header.
    m_headerWidget=new KNMusicPlaylistHeader(this);
    m_headerWidget->installEventFilter(this);
    setHeader(m_headerWidget);

    //Set activate signals.
    connect(this, &KNMusicPlaylistSongs::activated,
            this, &KNMusicPlaylistSongs::onItemActived);

    //Set mouse in/out animation time line.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndFrame(0x30);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistSongs::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setEndFrame(minGrey);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistSongs::changeBackground);

    //Set delegate and edit triggers.
    setItemDelegateForColumn(KNMusicGlobal::MusicDataCount,
                             new KNMusicPlaylistIndex(this));
}

void KNMusicPlaylistSongs::resetHeader()
{
    KNMusicListViewBase::resetHeader();
    //Here is a hack, music data count is now become song ID.
    setColumnHidden(KNMusicGlobal::MusicDataCount, false);
}

void KNMusicPlaylistSongs::resetIndexColumn()
{
    moveToFirst(KNMusicGlobal::MusicDataCount);
    resizeColumnToContents(KNMusicGlobal::MusicDataCount);
}

void KNMusicPlaylistSongs::onItemActived(const QModelIndex &index)
{
    if(index.isValid())
    {
        emit requireOpenUrl(index);
    }
}


void KNMusicPlaylistSongs::enterEvent(QEvent *e)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    KNMusicListViewBase::enterEvent(e);
}

void KNMusicPlaylistSongs::leaveEvent(QEvent *e)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    KNMusicListViewBase::leaveEvent(e);
}

void KNMusicPlaylistSongs::changeBackground(int frameData)
{
    m_backgroundColor=QColor(frameData, frameData, frameData);
    m_palette.setColor(QPalette::AlternateBase, m_backgroundColor);
    m_palette.setColor(QPalette::Button, m_backgroundColor);
    int textParam=(frameData<<1)+159;
    m_palette.setColor(QPalette::Text, QColor(textParam,
                                              textParam,
                                              textParam,
                                              0xCF+frameData));
    setPalette(m_palette);
}
