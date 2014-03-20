#include <QAction>

#include "knmusicviewermenu.h"

KNMusicViewerMenu::KNMusicViewerMenu(QWidget *parent) :
    KNMenu(parent)
{
    retranslate();
    createActions();
}

void KNMusicViewerMenu::retranslate()
{
    m_actionTitle[Browse]=tr("Browse");
    m_actionTitle[Copy]=tr("Copy");
}

void KNMusicViewerMenu::retranslateAndSet()
{
    retranslate();
}

void KNMusicViewerMenu::createActions()
{
    for(int i=0; i<MusicActionCount; i++)
    {
        m_action[i]=new QAction(m_actionTitle[i] ,this);
        addAction(m_action[i]);
    }
}
