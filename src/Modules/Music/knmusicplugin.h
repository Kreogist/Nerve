#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include <QThread>

#include "knmusicglobal.h"

#include "../Base/knpluginbase.h"

class KNGlobal;
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

signals:

public slots:

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

    QThread m_collectThread,
            m_modelThread;
    QString m_musicDatabase;
};

#endif // KNMUSICPLUIN_H
