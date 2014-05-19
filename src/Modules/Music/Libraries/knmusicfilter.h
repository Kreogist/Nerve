#ifndef KNMUSICFILTER_H
#define KNMUSICFILTER_H

#include "../../../Modules/Base/knlibfilter.h"

class KNMusicGlobal;
class KNMusicFilter : public KNLibFilter
{
    Q_OBJECT
public:
    explicit KNMusicFilter(QObject *parent = 0);

signals:

public slots:

protected:
    int getType(const QString &suffix);

private:
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICFILTER_H
