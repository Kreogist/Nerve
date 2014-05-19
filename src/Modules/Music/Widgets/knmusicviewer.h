#ifndef KNMUSICVIEWER_H
#define KNMUSICVIEWER_H

#include <QList>
#include <QUrl>
#include <QModelIndex>
#include <QRegExp>

#include "../knmusicglobal.h"

#include "../../Base/knstdlibviewer.h"

class QEvent;
class QDragEnterEvent;
class QDropEvent;
class KNLibFilter;
class KNMusicModel;
class KNMusicListView;
class KNMusicArtistView;
class KNMusicAlbumView;
class KNMusicSortModel;
class KNMusicArtistModel;
class KNMusicAlbumModel;
class KNMusicGenreModel;
class KNMusicCategoryDetailModel;
class KNMusicAlbumDetailModel;
class KNMusicArtistSongs;
class KNMusicGenreSongs;
class KNMusicViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNMusicViewer(QWidget *parent = 0);
    void setDefaultHeader();
    void setModel(KNMusicModel *model);
    bool eventFilter(QObject *watched, QEvent *event);

signals:
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index,
                                KNMusicGlobal::MusicCategory currentMode);
    void requireOpenUrl(const QModelIndex &index);
    void requireDelete(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();
    void resort();
    void showIn(KNMusicGlobal::MusicCategory category,
                const QModelIndex &index);
    void deleteMusic(const QModelIndex &index);
    void onActionSearch(const QString &text);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void onActionLibraryViewShowContextMenu(const QPoint &position,
                                            const QModelIndex &index);
    void onActionArtistShowContextMenu(const QPoint &position,
                                       const QModelIndex &index);
    void onActionAlbumShowContextMenu(const QPoint &position,
                                      const QModelIndex &index);
    void onActionGenreShowContextMenu(const QPoint &position,
                                      const QModelIndex &index);
    void onActionListviewOpenUrl(const QModelIndex &index);
    void onActionArtistOpenUrl(const QModelIndex &index);
    void onActionAlbumOpenUrl(const QModelIndex &index);
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
    KNMusicAlbumDetailModel *m_albumDetails;

    KNMusicArtistSongs *m_artistSongView;
    KNMusicGenreSongs *m_genreSongView;

    KNMusicModel *m_sourceModel;
};

#endif // KNMUSICVIEWER_H
