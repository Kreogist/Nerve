#ifndef KNMUSICVIEWER_H
#define KNMUSICVIEWER_H

#include <QList>
#include <QUrl>
#include <QModelIndex>
#include <QRegExp>
#include <QThread>

#include "../knmusicglobal.h"
#include "../../Public/Base/knmusicbackend.h"

#include "../../Base/knstdlibviewer.h"

class QEvent;
class QDragEnterEvent;
class QSortFilterProxyModel;
class QSignalMapper;
class QDropEvent;
class QPropertyAnimation;
class KNMusicCategorySortFilterModel;
class KNMusicLibraryModelBase;
class KNMusicListView;
class KNMusicCategoryView;
class KNMusicAlbumView;
class KNMusicSortModel;
class KNMusicArtistModel;
class KNMusicAlbumModel;
class KNMusicGenreModel;
class KNMusicCategoryDetailModel;
class KNMusicAlbumDetailModel;
class KNMusicArtistSongs;
class KNMusicPlaylistView;
class KNMusicGenreSongs;
class KNMusicPlaylistManager;
class KNMusicViewerItem;
class KNMusicViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNMusicViewer(QWidget *parent = 0);
    ~KNMusicViewer();
    void setPlaylistManager(KNMusicPlaylistManager *manager);
    void setMusicModel(KNMusicLibraryModelBase *model);
    void setBackend(KNMusicBackend *backend);
    bool eventFilter(QObject *watched, QEvent *event);
    void setPlayWidget(QWidget *widget);
    void addListviewModePlugin(KNMusicViewerItem *plugin);

signals:
    void requireShowContextMenu(const QPoint &position,
                                int currentMode);
    void requireDelete(const QModelIndex &index);
    void requirePlayMusic(const QString &filePath);
    void requireSetProxy(QSortFilterProxyModel *model);
    void requireShowIn(const int &category,
                       const QModelIndex &index);
    void requireClearSearch();
    void requireResort();
    void requireSearch(const QString &text);
    void requireSetMusicModel(KNMusicLibraryModelBase *model);
    void requireSetBackend(KNMusicBackend *backend);
    void requireRemoveOriginal(const QModelIndex &index);
    void requireShowInSongsView(const QModelIndex &index);
    void requireShowInArtistView(const QModelIndex &index);
    void requireShowInAlbumView(const QModelIndex &index);
    void requireShowInGenreView(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();
    void resort();
    void showIn(const int &category,
                const QModelIndex &index);
    void showInCurrent(const QModelIndex &index);
    void deleteMusic(const QModelIndex &index);
    void deleteSelections();
    void onActionSearch(const QString &text);
    void onActionShowPlayer();
    void onActionHidePlayer();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onActionArtistShowContextMenu(const QPoint &position);
    void onActionAlbumShowContextMenu(const QPoint &position);
    void onActionGenreShowContextMenu(const QPoint &position);

private:
    void addPlugin(KNMusicViewerItem *plugin);
    enum MusicCategories
    {
        Songs,
        Artists,
        Albums,
        Genres,
        Playlists,
        MusicCategoriesCount
    };
    QString m_categoryCaption[MusicCategoriesCount];
    QPropertyAnimation *m_playerIn, *m_playerOut;
    QWidget *m_playerWidget;

    QSignalMapper *m_showInMapper;

    KNMusicCategoryView *m_artistView,
                      *m_genreView;
    KNMusicAlbumView *m_albumView;
    KNMusicPlaylistView *m_playlistView;

    KNMusicArtistModel *m_artistModel;
    KNMusicAlbumModel *m_albumModel;
    KNMusicGenreModel *m_genreModel;

    KNMusicCategoryDetailModel *m_artistDetails,
                               *m_genreDetails;
    KNMusicAlbumDetailModel *m_albumDetails;
    KNMusicCategorySortFilterModel *m_artistSortModel,
                                   *m_albumSortModel,
                                   *m_genreSortModel;

    KNMusicArtistSongs *m_artistSongView;
    KNMusicGenreSongs *m_genreSongView;

    KNMusicLibraryModelBase *m_musicModel;

    QThread m_listViewModelThread,
            m_artistModelThread,
            m_albumViewModelThread,
            m_albumSortModelThread,
            m_genreModelThread,
            m_artistSortModelThread,
            m_genreSortModelThread,
            m_artistDetailsThread,
            m_albumDetailsThread,
            m_genreDetailsThread;
};

#endif // KNMUSICVIEWER_H
