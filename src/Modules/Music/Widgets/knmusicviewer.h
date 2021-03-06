#ifndef KNMUSICVIEWER_H
#define KNMUSICVIEWER_H

#include <QLinkedList>
#include <QList>
#include <QUrl>
#include <QThread>
#include <QModelIndex>
#include <QSortFilterProxyModel>

#include "../knmusicglobal.h"
#include "../../Public/Base/knmusicbackend.h"
#include "../Base/knmusicplaylistmanagerbase.h"
#include "../Base/knmusicvieweritembase.h"

#include "../../Base/knstdlibviewer.h"

class QEvent;
class QPropertyAnimation;
class KNMusicLibraryModelBase;
class KNMusicViewerPlaylistItemBase;
class KNMusicViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNMusicViewer(QWidget *parent = 0);
    ~KNMusicViewer();
    void setMusicModel(KNMusicLibraryModelBase *model);
    bool eventFilter(QObject *watched, QEvent *event);
    void setPlayWidget(QWidget *widget);
    void addListViewPlugin(KNMusicViewerItemBase *plugin);
    void addArtistViewPlugin(KNMusicViewerItemBase *plugin);
    void addAlbumViewPlugin(KNMusicViewerItemBase *plugin);
    void addGenreViewPlugin(KNMusicViewerItemBase *plugin);
    void addPlaylistPlugin(KNMusicViewerPlaylistItemBase *plugin);

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
    void requireSetPlaylistManager(KNMusicPlaylistManagerBase *manager);
    void requireSetMusicModel(KNMusicLibraryModelBase *model);
    void requireSetBackend(KNMusicBackend *backend);
    void requireRemoveOriginal(const QModelIndex &index);
    void requireRemoveMusic(const QModelIndex &index);
    void requireShowInSongsView(const QModelIndex &index);
    void requireShowInArtistView(const QModelIndex &index);
    void requireShowInAlbumView(const QModelIndex &index);
    void requireShowInGenreView(const QModelIndex &index);
    void dragEntered();
    void dropped();

public slots:
    void retranslate();
    void retranslateAndSet();
    void showIn(const int &category, const QModelIndex &index);
    void showInCurrent(const QModelIndex &index);
    void deleteMusic(const QModelIndex &index);
    void deleteSelections();
    void onActionShowPlayer();
    void onActionHidePlayer();
    void showPlaylistDragList();
    void hidePlaylistDragList();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void addDatabasePlugin(KNMusicViewerItemBase *plugin);
    enum MusicCategories
    {
        Songs,
        Artists,
        Albums,
        Genres,
        Playlists,
        MusicCategoriesCount
    };
    QPropertyAnimation *m_playerIn, *m_playerOut, *m_playlistIn, *m_playlistOut;
    QWidget *m_playerWidget, *m_playlistListView;
    QLinkedList<KNMusicViewerItemBase *> m_itemList;
//    QLinkedList<QThread *> m_itemThreads;

    KNMusicLibraryModelBase *m_musicModel;
};

#endif // KNMUSICVIEWER_H
