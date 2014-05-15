#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include <QThread>

#include "knmusicglobal.h"

#include "../Base/knpluginbase.h"

class QEvent;
class KNGlobal;
class KNMusicDatabase;
class KNMusicModel;
class KNMusicViewer;
class KNMusicInfoCollector;
class KNMusicInfoCollectorManager;
class KNMusicSearcher;
class KNMusicViewerMenu;
class KNMusicDetailInfo;
class KNMusicPlugin : public KNPluginBase
{
    Q_OBJECT
public:
    explicit KNMusicPlugin(QObject *parent = 0);
    ~KNMusicPlugin();
    void applyPlugin();
    void writeDatabase();
    void readDatabase();
    bool eventFilter(QObject * watched, QEvent * event);

signals:

public slots:

protected:

private slots:
    void onActionShowContextMenu(const QPoint &position,
                         const QModelIndex &index,
                         KNMusicGlobal::MusicCategory currentMode);
    void onActionOpenUrl(const QModelIndex &index);
    void onActionGetInfo(const QString &filePath);

private:
    KNMusicViewer *m_musicViewer;
    KNGlobal *m_global;
    KNMusicModel *m_model;
    KNMusicInfoCollector *m_infoCollector;
    KNMusicInfoCollectorManager *m_infoCollectManager;
    KNMusicSearcher *m_searcher;
    KNMusicViewerMenu *m_libraryViewMenu;
    KNMusicDetailInfo *m_detailsDialog;
    KNMusicDatabase *m_database;

    QThread m_searcherThread,
            m_collectThread,
            m_modelThread,
            m_databaseThread;
    QString m_musicDatabase,
            m_musicAlbumArt;
};

#endif // KNMUSICPLUIN_H
