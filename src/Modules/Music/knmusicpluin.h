#ifndef KNMUSICPLUIN_H
#define KNMUSICPLUIN_H

#include "../Base/knpluginbase.h"

class KNGlobal;
class KNMusicViewer;
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
};

#endif // KNMUSICPLUIN_H
