#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include "../Base/knpluginbase.h"

class KNGlobal;
class KNMusicModel;
class KNMusicViewer;
class KNMusicInfoCollector;
class KNMusicSearcher;
class KNMusicPluin : public KNPluginBase
{
    Q_OBJECT
public:
    explicit KNMusicPluin(QObject *parent = 0);
    void applyPlugin();

signals:

public slots:

private:
    KNMusicViewer *m_musicViewer;
    KNGlobal *m_global;
    KNMusicModel *m_model;
    KNMusicInfoCollector *m_infoCollector;
    KNMusicSearcher *m_searcher;
};

#endif // KNMUSICPLUIN_H
