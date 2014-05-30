#include <QBoxLayout>

#include "../../Base/knsearchbox.h"
#include "../Libraries/knmusicmodel.h"
#include "../knmusicglobal.h"
#include "knmusicheaderplayer.h"

#include "knmusicheaderwidget.h"

KNMusicHeaderWidget::KNMusicHeaderWidget(QWidget *parent) :
    KNStdLibHeaderWidget(parent)
{
    setContentsMargins(0,0,0,0);
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_mainLayout->setAlignment(Qt::AlignVCenter);
    setLayout(m_mainLayout);

    m_headerPlayer=new KNMusicHeaderPlayer(this);
    m_mainLayout->addWidget(m_headerPlayer, 0, Qt::AlignLeft | Qt::AlignVCenter);

    m_searchBox=new KNSearchBox(this);
    m_searchBox->setFixedWidth(200);
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
    connect(m_searchBox, &KNSearchBox::textChanged,
            this, &KNMusicHeaderWidget::requireSearch);
    connect(m_searchBox, &KNSearchBox::requireLostFocus,
            this, &KNMusicHeaderWidget::requireLostFocus);
    m_mainLayout->addWidget(m_searchBox, 0, Qt::AlignRight | Qt::AlignVCenter);
}

void KNMusicHeaderWidget::setMusicModel(KNMusicModel *model)
{
    m_musicModel=model;
}

void KNMusicHeaderWidget::setMusicPlayer(KNMusicPlayer *player)
{
    m_headerPlayer->setPlayer(player);
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
}

void KNMusicHeaderWidget::onActionPlayMusic(const QModelIndex &index)
{
    m_headerPlayer->setAlbumArt(QPixmap::fromImage(m_musicModel->artwork(index.row())));
    m_headerPlayer->setTitle(m_musicModel->itemText(index.row(), KNMusicGlobal::Name));
    m_headerPlayer->setArtist(m_musicModel->itemText(index.row(), KNMusicGlobal::Artist));
    m_headerPlayer->playFile(m_musicModel->filePathFromIndex(index.row()));
}
