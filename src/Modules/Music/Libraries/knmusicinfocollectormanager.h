#ifndef KNMUSICINFOCOLLECTORMANAGER_H
#define KNMUSICINFOCOLLECTORMANAGER_H

#include <QThread>
#include <QStringList>
#include <QList>
#include <QModelIndex>

#include "../knmusicglobal.h"

#include "../../Base/knlibinfocollectormanager.h"

class KNMusicInfoCollector;
class KNMusicInfoCollectorManager : public KNLibInfoCollectorManager
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollectorManager(QObject *parent = 0);
    ~KNMusicInfoCollectorManager();

    QStringList currentFileData() const;
    KNMusicGlobal::MusicDetailsInfo currentFileAppendData() const;

signals:
    void requireAnalysis(const QString &filePath);

public slots:
    void addAnalysisList(const QModelIndex &index,
                         const QString &filePath);

private slots:
    void currentSkip();
    void currentWorkDone(const QStringList &value,
                         const KNMusicGlobal::MusicDetailsInfo &datas);

private:
    struct AnalysisQueueItem
    {
        QString filePath;
        QModelIndex index;
    };

    KNMusicInfoCollector *m_collector;
    QThread m_collectThread;
    QList<AnalysisQueueItem> m_analysisQueue;
    QList<QModelIndex> m_indexList;
    QStringList m_fileList, m_currentFileData;
    KNMusicGlobal::MusicDetailsInfo m_currentFileAppendData;
    bool m_working=false;
};

#endif // KNMUSICINFOCOLLECTORMANAGER_H
