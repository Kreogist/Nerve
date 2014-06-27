#ifndef KNMUSICSEARCHER_H
#define KNMUSICSEARCHER_H

#include "../../Base/knlibsearcher.h"

class KNMusicGlobal;
class KNMusicSearcher : public KNLibSearcher
{
    Q_OBJECT
public:
    explicit KNMusicSearcher(QObject *parent = 0);

signals:

public slots:

protected:
    int getType(const QString &suffix);

private:
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICSEARCHER_H
