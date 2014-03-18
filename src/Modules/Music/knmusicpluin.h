#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include "../Base/knpluginbase.h"

class KNGlobal;
class KNMusicModel;
class KNMusicViewer;
class KNMusicInfoCollector;
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
    KNMusicInfoCollector *m_inforCollector;
};

#endif // KNMUSICPLUIN_H
