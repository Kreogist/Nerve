#include <QStyleFactory>

#include "../../knlocale.h"

#include "knmusiclistview.h"

KNMusicListView::KNMusicListView(QWidget *parent) :
    QTreeView(parent)
{
    retranslate();
    m_locale=KNLocale::instance();
    connect(m_locale, SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));

    setStyle(QStyleFactory::create("fusion"));
}

void KNMusicListView::retranslate()
{
    ;
}

void KNMusicListView::retranslateAndSet()
{
    retranslate();
}
