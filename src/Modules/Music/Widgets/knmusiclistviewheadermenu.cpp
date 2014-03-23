#include <QAction>
#include <QSignalMapper>

#include "knmusiclistviewheadermenu.h"

KNMusicListViewHeaderMenu::KNMusicListViewHeaderMenu(QWidget *parent) :
    KNMenu(parent)
{
    retranslate();
    m_musicGlobal=KNMusicGlobal::instance();
    m_headerVisibleChange=new QSignalMapper(this);
    connect(m_headerVisibleChange, SIGNAL(mapped(int)),
            this, SLOT(onActionVisibleChanged(int)));
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        m_headerVisible[i]=new QAction(m_musicGlobal->getHeader(i),
                                     this);
        m_headerVisible[i]->setCheckable(true);
        connect(m_headerVisible[i], SIGNAL(triggered()),
                m_headerVisibleChange, SLOT(map()));
        m_headerVisibleChange->setMapping(m_headerVisible[i], i);
        addAction(m_headerVisible[i]);
    }
}

void KNMusicListViewHeaderMenu::setHeaderValue(const int &index,
                                               const bool &visible)
{
    m_headerVisible[index]->setChecked(visible);
}

void KNMusicListViewHeaderMenu::retranslate()
{
    ;
}

void KNMusicListViewHeaderMenu::retranslateAndSet()
{
    retranslate();
}

void KNMusicListViewHeaderMenu::onActionVisibleChanged(const int &index)
{
    emit requireChangeVisible(index, m_headerVisible[index]->isChecked());
}
