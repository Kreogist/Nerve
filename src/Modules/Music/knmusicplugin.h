#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include <QThread>

#include "knmusicglobal.h"

#include "../Base/knpluginbase.h"

class QAction;
class KNGlobal;
class KNMusicPlayer;
class KNMusicPlayerWidget;
class KNMusicDatabase;
class KNMusicHeaderWidget;
class KNMusicModel;
class KNMusicViewer;
class KNMusicInfoCollector;
class KNMusicInfoCollectorManager;
class KNMusicFilter;
class KNMusicViewerMenu;
class KNMusicEQ;
class KNMusicDetailInfo;
class KNMusicPlugin : public KNPluginBase
{
    Q_OBJECT
public:
    explicit KNMusicPlugin(QObject *parent = 0);
    ~KNMusicPlugin();
    void applyPlugin();

signals:

public slots:

protected:

private slots:
    void onActionShowContextMenu(const QPoint &position,
                         const QModelIndex &index,
                         KNMusicGlobal::MusicCategory currentMode);
    void onActionOpenUrl(const QModelIndex &index);
    void onActionPlay(const QModelIndex &index);
    void onActionGetInfo(const QString &filePath);

private:
    void createShortcuts();
    KNMusicViewer *m_musicViewer;
    KNGlobal *m_global;
    KNMusicModel *m_model;
    KNMusicInfoCollector *m_infoCollector;
    KNMusicInfoCollectorManager *m_infoCollectManager;
    KNMusicFilter *m_searcher;
    KNMusicViewerMenu *m_libraryViewMenu;
    KNMusicDetailInfo *m_detailsDialog;
    KNMusicDatabase *m_database;
    KNMusicPlayer *m_musicPlayer;
    KNMusicEQ *m_equalizer;
    KNMusicHeaderWidget *m_headerWidget;
    KNMusicPlayerWidget *m_musicPlayerWidget;

    QThread m_searcherThread,
            m_collectThread,
            m_modelThread,
            m_databaseThread,
            m_playerThread;
    QString m_musicDatabase,
            m_musicAlbumArt;
};

#endif // KNMUSICPLUIN_H
