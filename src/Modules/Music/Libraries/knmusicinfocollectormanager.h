#ifndef KNMUSICINFOCOLLECTORMANAGER_H
#define KNMUSICINFOCOLLECTORMANAGER_H

#include <QThread>
#include <QStringList>

#include "../knmusicglobal.h"

#include "../../Base/knlibinfocollectormanager.h"

class KNMusicInfoCollector;
class KNMusicInfoCollectorManager : public KNLibInfoCollectorManager
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollectorManager(QObject *parent = 0);
    ~KNMusicInfoCollectorManager();

signals:
    void requireAppendMusic(const QStringList &value,
                            const KNMusicGlobal::MusicDetailsInfo &datas);
    void requireSortData();

public slots:
    void addAnalysisList(const QString &filePath);

private slots:
    void currentSkip();
    void currentWorkDone(const QStringList &value,
                         const KNMusicGlobal::MusicDetailsInfo &datas);

private:
    KNMusicInfoCollector *m_collector;
    QThread m_collectThread;
    QStringList m_fileList;
    bool m_working=false;
};

#endif // KNMUSICINFOCOLLECTORMANAGER_H
