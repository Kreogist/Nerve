#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include <QThread>

#include "../Base/knpluginbase.h"

class KNGlobal;
class KNMusicModel;
class KNMusicViewer;
class KNMusicInfoCollector;
class KNMusicInfoCollectorManager;
class KNMusicSearcher;
class KNMusicViewerMenu;
class KNMusicPluin : public KNPluginBase
{
    Q_OBJECT
public:
    explicit KNMusicPluin(QObject *parent = 0);
    ~KNMusicPluin();
    void applyPlugin();
    void writeDatabase();
    void readDatabase();

signals:

public slots:

private slots:
    void showContextMenu(const QPoint &position,
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
