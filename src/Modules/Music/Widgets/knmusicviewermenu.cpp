#include <QAction>

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
    m_filePath=filePath;
}

void KNMusicViewerMenu::retranslate()
{
#ifdef Q_OS_WIN
    m_actionTitle[Browse]=tr("Show in Explorer");
#endif
#ifdef Q_OS_MAC
    m_actionTitle[Browse]=tr("Show in Finder");
#endif
    m_actionTitle[Copy]=tr("Copy");
}

void KNMusicViewerMenu::retranslateAndSet()
{
    retranslate();
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
    connect(m_action[Browse], SIGNAL(triggered()),
            this, SLOT(onActionBrowse()));
}
