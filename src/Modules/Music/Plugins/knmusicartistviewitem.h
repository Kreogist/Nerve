#ifndef KNMUSICARTISTVIEWITEM_H
#define KNMUSICARTISTVIEWITEM_H

#include "../Base/knmusicvieweritembase.h"

class KNMusicCategoryView;
class KNMusicArtistModel;
class KNMusicCategoryDetailModel;
class KNMusicCategorySortFilterModel;
class KNMusicArtistSongs;
class KNMusicArtistViewItem : public KNMusicViewerItemBase
{
    Q_OBJECT
public:
    explicit KNMusicArtistViewItem(QObject *parent = 0);
    ~KNMusicArtistViewItem();
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
    KNMusicCategoryView *m_artistView;
    KNMusicArtistModel *m_artistModel;
    KNMusicCategoryDetailModel *m_artistDetails;
    KNMusicCategorySortFilterModel *m_artistSortModel;
    KNMusicArtistSongs *m_artistSongView;
    QString m_captionTitle;
};

#endif // KNMUSICARTISTVIEWITEM_H
