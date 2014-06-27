#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include <QThread>
#include <QList>
#include <QUrl>

#include "knmusicglobal.h"

#include "../Base/knpluginbase.h"

class QAction;
class KNGlobal;
class KNLibSearcher;
class KNMusicBackend;
class KNMusicPlayerWidget;
class KNMusicDatabaseBase;
class KNMusicHeaderWidget;
class KNMusicLibraryModel;
class KNMusicPlaylistManager;
class KNMusicViewer;
class KNMusicInfoCollector;
class KNMusicInfoCollectorManager;
class KNMusicSearcher;
class KNMusicViewerMenu;
class KNMusicEQ;
class KNMusicDetailInfoBase;
class KNMusicPlugin : public KNPluginBase
{
    Q_OBJECT
public:
    explicit KNMusicPlugin(QObject *parent = 0);
    ~KNMusicPlugin();
    void applyPlugin();

signals:
    void requireAnalysisUrls(const QList<QUrl> urls);
    void requireAddRawFiles(const QStringList &fileList);

public slots:

protected:

private slots:
    void onActionShowContextMenu(const QPoint &position,
                                 KNMusicGlobal::MusicCategory currentMode);
    void onActionOpenUrl(const QModelIndex &index);
    void onActionGetInfo(const QString &filePath);

private:
    void setSearcher(KNLibSearcher *searcher);
    void setDatabase(KNMusicDatabaseBase *database);
    void setDetailsDialog(KNMusicDetailInfoBase *detailInfoDialog);
    void setListviewMenu();
    void loadThreads();
    void loadData();
    void loadShortcuts();
    KNMusicViewer *m_musicViewer;
    KNGlobal *m_global;
    KNMusicLibraryModel *m_libraryModel;
    KNMusicInfoCollector *m_infoCollector;
    KNMusicInfoCollectorManager *m_infoCollectManager;
    KNLibSearcher *m_searcher;
    KNMusicViewerMenu *m_libraryViewMenu;
    KNMusicDetailInfoBase *m_detailsDialog;
    KNMusicDatabaseBase *m_musicDatabase;
    KNMusicBackend *m_musicPlayer;
    KNMusicEQ *m_equalizer;
    KNMusicHeaderWidget *m_headerWidget;
    KNMusicPlayerWidget *m_musicPlayerWidget;
    KNMusicPlaylistManager *m_playlistManager;

    QThread m_searcherThread,
            m_collectThread,
            m_modelThread,
            m_databaseThread,
            m_playerThread;
    QString m_musicDatabasePath,
            m_musicAlbumArt;
};

#endif // KNMUSICPLUIN_H
