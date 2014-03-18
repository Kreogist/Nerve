#include <QDebug>

#include <QStyleFactory>

#include "../../knlocale.h"

#include "knmusiclistviewheader.h"

#include "knmusiclistview.h"

KNMusicListView::KNMusicListView(QWidget *parent) :
    QTreeView(parent)
{
    //Set properties.
    setStyle(QStyleFactory::create("fusion"));

    //Set locale.
    retranslate();
    connect(KNLocale::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));
}

void KNMusicListView::retranslate()
{


}

void KNMusicListView::retranslateAndSet()
{
    retranslate();
}
