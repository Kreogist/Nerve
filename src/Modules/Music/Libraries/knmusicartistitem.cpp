#include "knmusicartistitem.h"

KNMusicArtistItem::KNMusicArtistItem(QString name) :
    QStandardItem(name)
{
}

bool KNMusicArtistItem::hasIcon() const
{
    return m_hasIcon;
}

void KNMusicArtistItem::setHasIcon(const bool &value)
{
    m_hasIcon=value;
}
