#include <QAbstractItemModel>
#include <QListView>

#include "knmusicartistview.h"

KNMusicArtistView::KNMusicArtistView(QWidget *parent) :
    QWidget(parent)
{
    m_artistList=new QListView(this);
    m_artistList->setIconSize(QSize(40, 40));
}

void KNMusicArtistView::setModel(QAbstractItemModel *model)
{
    m_artistList->setModel(model);
}
