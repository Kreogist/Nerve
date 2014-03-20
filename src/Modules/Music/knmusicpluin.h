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
class KNMusicPluin : public KNPluginBase
{
    Q_OBJECT
public:
    explicit KNMusicPluin(QObject *parent = 0);
    ~KNMusicPluin();
    void applyPlugin();

signals:

public slots:

private:
    KNMusicViewer *m_musicViewer;
    KNGlobal *m_global;
    KNMusicModel *m_model;
    KNMusicInfoCollector *m_infoCollector;
    KNMusicInfoCollectorManager *m_infoCollectManager;
    KNMusicSearcher *m_searcher;

    QThread m_collectThread,
            m_modelThread;
};

#endif // KNMUSICPLUIN_H
