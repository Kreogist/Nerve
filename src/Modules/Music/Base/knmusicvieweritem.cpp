#include "knmusicvieweritembase.h"

KNMusicViewerItemBase::KNMusicViewerItemBase(QObject *parent) :
    QObject(parent)
{
}

void KNMusicViewerItemBase::onActionResort()
{
}

void KNMusicViewerItemBase::onActionShowContextMenu(const QPoint &position)
{
    Q_UNUSED(position);
}

void KNMusicViewerItemBase::onActionSearch(const QString &text)
{
    Q_UNUSED(text);
}

void KNMusicViewerItemBase::onActionShowIndex(const QModelIndex &index)
{
    Q_UNUSED(index);
}

void KNMusicViewerItemBase::onActionRemoveItem(const QModelIndex &index)
{
    Q_UNUSED(index);
}

void KNMusicViewerItemBase::onActionRemoveOriginalItem(const QModelIndex &index)
{
    Q_UNUSED(index);
}
