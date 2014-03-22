#include <QAbstractItemModel>
#include <QListView>
#include <QSplitter>
#include <QScrollBar>
#include <QLabel>
#include <QBoxLayout>

#include "knmusicartistview.h"

KNMusicArtistDetailsDisplay::KNMusicArtistDetailsDisplay(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(0,0,0,0);
    QBoxLayout *m_layout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(m_layout);

    m_artistName=new QLabel(this);
    m_layout->addWidget(m_artistName);
    m_artistInfo=new QLabel(this);
    m_layout->addWidget(m_artistInfo);
}

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

    m_artistDetails=new KNMusicArtistDetailsDisplay(this);
    addWidget(m_artistDetails);
    setStretchFactor(1, 1);
}

void KNMusicArtistView::setModel(QAbstractItemModel *model)
{
    m_artistList->setModel(model);
}

void KNMusicArtistView::resort()
{
    m_artistList->model()->sort(0);
}
