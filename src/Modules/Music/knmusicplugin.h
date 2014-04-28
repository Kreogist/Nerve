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
    void showContextMenu(KNMusicGlobal::MusicCategory category,
                         const QPoint &position,
                         const QModelIndex &index);
    void onActionOpenUrl(const QModelIndex &index);

private:
    KNMusicViewer *m_musicViewer;
    KNGlobal *m_global;
    KNMusicModel *m_model;
    KNMusicInfoCollector *m_infoCollector;
    KNMusicInfoCollectorManager *m_infoCollectManager;
    KNMusicSearcher *m_searcher;
    KNMusicViewerMenu *m_libraryViewMenu;

    QThread m_collectThread,
            m_modelThread;
    QString m_musicDatabase;
};

#endif // KNMUSICPLUIN_H
