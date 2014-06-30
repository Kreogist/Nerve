#ifndef KNMUSICALBUMVIEWITEM_H
#define KNMUSICALBUMVIEWITEM_H

#include "../Base/knmusicvieweritem.h"

class KNMusicAlbumView;
class KNMusicAlbumModel;
class KNMusicAlbumDetailModel;
class KNMusicCategorySortFilterModel;
class KNMusicAlbumViewItem : public KNMusicViewerItem
{
    Q_OBJECT
public:
    explicit KNMusicAlbumViewItem(QObject *parent = 0);
    ~KNMusicAlbumViewItem();
    void applyPlugin();

signals:

public slots:
    void onActionResort();
    void onActionSearch(const QString &text);
    void onActionShowIndex(const QModelIndex &index);
    void onActionRemoveItem(const QModelIndex &index);
    void retranslate();
    void setMusicSourceModel(KNMusicLibraryModelBase *model);
    void setBackend(KNMusicBackend *backend);

protected slots:
    void onActionShowContextMenu(const QPoint &position);

private:
    KNMusicAlbumView *m_albumView;
    KNMusicAlbumModel *m_albumModel;
    KNMusicAlbumDetailModel *m_albumDetails;
    KNMusicCategorySortFilterModel *m_albumSortModel;
    QString m_captionTitle;
};

#endif // KNMUSICALBUMVIEWITEM_H
