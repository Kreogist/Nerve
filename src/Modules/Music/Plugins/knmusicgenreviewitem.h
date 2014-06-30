#ifndef KNMUSICGENREVIEWITEM_H
#define KNMUSICGENREVIEWITEM_H

#include "../Base/knmusicvieweritembase.h"

class KNMusicCategoryView;
class KNMusicGenreModel;
class KNMusicCategoryDetailModel;
class KNMusicCategorySortFilterModel;
class KNMusicGenreSongs;
class KNMusicGenreViewItem : public KNMusicViewerItemBase
{
    Q_OBJECT
public:
    explicit KNMusicGenreViewItem(QObject *parent = 0);
    ~KNMusicGenreViewItem();
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
    KNMusicCategoryView *m_genreView;
    KNMusicGenreModel *m_genreModel;
    KNMusicCategoryDetailModel *m_genreDetails;
    KNMusicCategorySortFilterModel *m_genreSortModel;
    KNMusicGenreSongs *m_genreSongView;
    QString m_captionTitle;
};

#endif // KNMUSICGENREVIEWITEM_H
