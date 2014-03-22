#include <QAbstractItemModel>
#include <QListView>
#include <QSplitter>
#include <QScrollBar>

#include "knmusicartistview.h"

KNMusicArtistView::KNMusicArtistView(QWidget *parent) :
    QSplitter(parent)
{
    setContentsMargins(0,0,0,0);
    setHandleWidth(2);

    m_artistList=new QListView(this);
    m_artistList->setIconSize(QSize(40, 40));
    m_artistList->setUniformItemSizes(true);
    m_artistList->setSpacing(1);
    m_artistList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_artistList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    addWidget(m_artistList);

    QWidget *test=new QWidget(this);
    addWidget(test);
}

void KNMusicArtistView::setModel(QAbstractItemModel *model)
{
    m_artistList->setModel(model);
}

void KNMusicArtistView::resort()
{
    m_artistList->model()->sort(0);
}
