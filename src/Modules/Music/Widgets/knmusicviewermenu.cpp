#include <QAction>
#include <QFileInfo>
#include <QStandardItem>
#include <QStandardItemModel>

#include <QDebug>

#include "../../knglobal.h"

#include "knmusicviewermenu.h"

KNMusicViewerMenu::KNMusicViewerMenu(QWidget *parent) :
    KNMenu(parent)
{
    m_global=KNGlobal::instance();
    retranslate();
    createActions();
}

void KNMusicViewerMenu::setItemIndex(const QModelIndex &index)
{
    m_currentIndex=index;
    QStandardItem *item=m_model->itemFromIndex(m_currentIndex);

    m_filePath=m_model->item(m_currentIndex.row(),
                             KNMusicGlobal::Name)->data(Qt::UserRole).toString();
    QFileInfo currentFile(m_filePath);
    QString shownFileName=currentFile.fileName();
    shownFileName.replace("&", "&&");
    m_action[Play]->setText(m_actionTitle[Play].arg(shownFileName));

    m_itemText=item->text();
    if(m_itemText.isEmpty())
    {
        m_action[CopyText]->setVisible(false);
        return;
    }
    m_action[CopyText]->setVisible(true);
    m_action[CopyText]->setText(m_actionTitle[CopyText].arg(m_itemText));
}

void KNMusicViewerMenu::retranslate()
{
    m_actionTitle[Play]=tr("Play %1");
    m_actionTitle[ShowInSongs]=tr("Go to Songs");
    m_actionTitle[ShowInArtist]=tr("Go to Artist");
    m_actionTitle[ShowInAlbum]=tr("Go to Album");
    m_actionTitle[ShowInGenre]=tr("Go to Genre");
#ifdef Q_OS_WIN
    m_actionTitle[Browse]=tr("Show in Explorer");
#endif
#ifdef Q_OS_MAC
    m_actionTitle[Browse]=tr("Show in Finder");
#endif
#ifdef Q_OS_LINUX
    m_actionTitle[Browse]=tr("Show the contains folder");
#endif
    m_actionTitle[Copy]=tr("Copy File");
    m_actionTitle[CopyText]=tr("Copy '%1'");
}

void KNMusicViewerMenu::retranslateAndSet()
{
    retranslate();
}

void KNMusicViewerMenu::onActionCopy()
{
    m_global->copyFileToClipboard(QStringList(m_filePath));
}

void KNMusicViewerMenu::onActionPlay()
{
    m_global->openLocalUrl(m_filePath);
}

void KNMusicViewerMenu::onActionCopyText()
{
    m_global->copyTextToClipboard(m_itemText);
}

void KNMusicViewerMenu::onActionBrowse()
{
    m_global->showInGraphicalShell(m_filePath);
}

void KNMusicViewerMenu::onActionShowInSongs()
{
    emit requireShowIn(KNMusicGlobal::SongsView,
                       m_currentIndex);
}

void KNMusicViewerMenu::onActionShowInArtist()
{
    emit requireShowIn(KNMusicGlobal::ArtistView,
                       m_currentIndex);
}

void KNMusicViewerMenu::onActionShowInAlbum()
{
    emit requireShowIn(KNMusicGlobal::AlbumView,
                       m_currentIndex);
}

void KNMusicViewerMenu::onActionShowInGenre()
{
    emit requireShowIn(KNMusicGlobal::GenreView,
                       m_currentIndex);
}

void KNMusicViewerMenu::createActions()
{
    for(int i=0; i<MusicActionCount; i++)
    {
        m_action[i]=new QAction(m_actionTitle[i] ,this);
        addAction(m_action[i]);
    }
    connect(m_action[Play], SIGNAL(triggered()),
            this, SLOT(onActionPlay()));
    connect(m_action[ShowInSongs], SIGNAL(triggered()),
            this, SLOT(onActionShowInSongs()));
    connect(m_action[ShowInArtist], SIGNAL(triggered()),
            this, SLOT(onActionShowInArtist()));
    connect(m_action[ShowInAlbum], SIGNAL(triggered()),
            this, SLOT(onActionShowInAlbum()));
    connect(m_action[ShowInGenre], SIGNAL(triggered()),
            this, SLOT(onActionShowInGenre()));
    connect(m_action[Browse], SIGNAL(triggered()),
            this, SLOT(onActionBrowse()));
    connect(m_action[Copy], SIGNAL(triggered()),
            this, SLOT(onActionCopy()));
    connect(m_action[CopyText], SIGNAL(triggered()),
            this, SLOT(onActionCopyText()));
}

void KNMusicViewerMenu::setModel(QStandardItemModel *model)
{
    m_model=model;
}

void KNMusicViewerMenu::setMode(KNMusicGlobal::MusicCategory category)
{
    for(int i=ShowInSongs;
        i<=ShowInGenre;
        i++)
    {
        m_action[i]->setVisible(true);
    }
    m_action[ShowInSongs+category]->setVisible(false);
}

