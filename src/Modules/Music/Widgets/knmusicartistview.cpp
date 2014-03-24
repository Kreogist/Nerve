#include <QAbstractItemModel>
#include <QListView>
#include <QSplitter>
#include <QScrollBar>
#include <QLabel>
#include <QBoxLayout>

#include <QDebug>

#include "../Libraries/knmusicartistitem.h"
#include "../Libraries/knmusicartistmodel.h"

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

    m_song=tr("1 song");
    m_songs=tr("%1 songs");
}

void KNMusicArtistDetailsDisplay::setModel(KNMusicArtistModel *model)
{
    m_model=model;
}

void KNMusicArtistDetailsDisplay::setArtistName(const QString &artistName)
{
    m_artistName->setText(artistName);
}

void KNMusicArtistDetailsDisplay::setSongNumber(const int &index)
{
    if(index==1)
    {
        m_artistInfo->setText(m_song);
    }
    else
    {
        m_artistInfo->setText(m_songs.arg(QString::number(index)));
    }
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
    m_artistList->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_artistList, SIGNAL(clicked(QModelIndex)),
            this, SLOT(onActionItemActivate(QModelIndex)));
    addWidget(m_artistList);

    m_artistDetails=new KNMusicArtistDetailsDisplay(this);
    addWidget(m_artistDetails);
    setStretchFactor(1, 1);
}

void KNMusicArtistView::setModel(KNMusicArtistModel *model)
{
    m_artistList->setModel(model);
    m_artistModel=model;
}

void KNMusicArtistView::resort()
{
    m_artistList->model()->sort(0);
}

void KNMusicArtistView::onActionItemActivate(const QModelIndex &index)
{
    m_artistDetails->setArtistName(m_artistModel->data(index,Qt::DisplayRole).toString());
    KNMusicArtistItem *currentArtist=
            static_cast<KNMusicArtistItem *>(m_artistModel->itemFromIndex(index));
    m_artistDetails->setSongNumber(currentArtist->songs().count());
}
