#ifndef KNMUSICVIEWER_H
#define KNMUSICVIEWER_H

#include <QList>
#include <QUrl>
#include <QModelIndex>

#include "../knmusicglobal.h"

#include "../../Base/knstdlibviewer.h"

class QDragEnterEvent;
class QDropEvent;
class KNMusicListView;
class KNMusicArtistView;
class KNMusicAlbumView;
class KNMusicSortModel;
class KNMusicArtistModel;
class KNMusicAlbumModel;
class KNMusicGenreModel;
class KNMusicCategoryDetailModel;
class KNMusicViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNMusicViewer(QWidget *parent = 0);
    void setDefaultHeader();
    void setModel(QAbstractItemModel *model);

signals:
    void requireAnalysisUrls(QList<QUrl> urls);
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index,
                                KNMusicGlobal::MusicCategory currentMode);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();
    void resort();
    void showIn(KNMusicGlobal::MusicCategory category,
                const QModelIndex &index);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void onActionLibraryViewShowContextMenu(const QPoint &position,
                                            const QModelIndex &index);
    void onActionArtistShowContextMenu(const QPoint &position,
                                       const QModelIndex &index);
    void onActionGenreShowContextMenu(const QPoint &position,
                                      const QModelIndex &index);
    void onActionListviewOpenUrl(const QModelIndex &index);
    void onActionArtistOpenUrl(const QModelIndex &index);
    void onActionGenreOpenUrl(const QModelIndex &index);

private:
    enum MusicCategories
    {
        Songs,
        Artists,
        Albums,
        Genres,
        MusicCategoriesCount
    };
    QString m_categoryCaption[MusicCategoriesCount];
    KNMusicListView *m_libraryView;
    KNMusicArtistView *m_artistView,
                      *m_genreView;
    KNMusicAlbumView *m_albumView;
    KNMusicSortModel *m_listViewModel;
    KNMusicArtistModel *m_artistModel;
    KNMusicAlbumModel *m_albumModel;
    KNMusicGenreModel *m_genreModel;
    KNMusicCategoryDetailModel *m_artistDetails,
                               *m_genreDetails;
    QAbstractItemModel *m_sourceModel;
};

#endif // KNMUSICVIEWER_H
