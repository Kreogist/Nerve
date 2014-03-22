#include <QAbstractItemModel>
#include <QListView>
#include <QSplitter>

#include "knmusicartistview.h"

KNMusicArtistView::KNMusicArtistView(QWidget *parent) :
    QSplitter(parent)
{
    setContentsMargins(0,0,0,0);
    setHandleWidth(0);

    m_artistList=new QListView(this);
    m_artistList->setIconSize(QSize(40, 40));

    addWidget(m_artistList);

    QWidget *test=new QWidget(this);
    addWidget(test);
}

void KNMusicArtistView::setModel(QAbstractItemModel *model)
{
    m_artistList->setModel(model);
}
