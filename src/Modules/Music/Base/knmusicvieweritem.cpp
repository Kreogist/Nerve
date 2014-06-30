#include "knmusicvieweritem.h"

KNMusicViewerItem::KNMusicViewerItem(QObject *parent) :
    QObject(parent)
{
}

void KNMusicViewerItem::onActionResort()
{
}

void KNMusicViewerItem::onActionShowContextMenu(const QPoint &position)
{
    Q_UNUSED(position);
}

void KNMusicViewerItem::onActionSearch(const QString &text)
{
    Q_UNUSED(text);
}

void KNMusicViewerItem::onActionShowIndex(const QModelIndex &index)
{
    Q_UNUSED(index);
}

void KNMusicViewerItem::onActionRemoveOriginalItem(const QModelIndex &index)
{
    Q_UNUSED(index);
}
