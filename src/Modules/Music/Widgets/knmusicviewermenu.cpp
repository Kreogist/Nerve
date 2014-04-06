#include <QAction>
#include <QFileInfo>
#include <QStandardItem>

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

void KNMusicViewerMenu::setFilePath(const QString &filePath)
{
    QFileInfo currentFile(filePath);
    m_filePath=filePath;
    QString shownFileName=currentFile.fileName();
    shownFileName.replace("&", "&&");
    m_action[Play]->setText(m_actionTitle[Play].arg(shownFileName));
}

void KNMusicViewerMenu::setItem(const QStandardItem *item)
{
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
#ifdef Q_OS_WIN
    m_actionTitle[Browse]=tr("Show in Explorer");
#endif
#ifdef Q_OS_MAC
    m_actionTitle[Browse]=tr("Show in Finder");
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

void KNMusicViewerMenu::createActions()
{
    for(int i=0; i<MusicActionCount; i++)
    {
        m_action[i]=new QAction(m_actionTitle[i] ,this);
        addAction(m_action[i]);
    }
    connect(m_action[Play], SIGNAL(triggered()),
            this, SLOT(onActionPlay()));
    connect(m_action[Browse], SIGNAL(triggered()),
            this, SLOT(onActionBrowse()));
    connect(m_action[Copy], SIGNAL(triggered()),
            this, SLOT(onActionCopy()));
    connect(m_action[CopyText], SIGNAL(triggered()),
            this, SLOT(onActionCopyText()));
}
