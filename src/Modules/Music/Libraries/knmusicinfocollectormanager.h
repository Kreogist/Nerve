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
    int currentIndex() const;
    void removeFirstUpdateResult();
    bool isUpdateQueueEmpty();

signals:
    void requireAnalysis(const QString &filePath);

public slots:
    void addAnalysisList(int index,
                         QString filePath);

private slots:
    void currentSkip();
    void currentWorkDone(QStringList value,
                         KNMusicGlobal::MusicDetailsInfo datas);

private:
    struct AnalysisQueueItem
    {
        int index;
        QString filePath;
    };
    struct ResultQueueItem
    {
        int index;
        QStringList text;
        KNMusicGlobal::MusicDetailsInfo details;
    };

    KNMusicInfoCollector *m_collector;
    QThread m_collectThread;
    QList<AnalysisQueueItem> m_analysisQueue;
    QList<QModelIndex> m_indexList;
    QList<ResultQueueItem> m_resultQueue;
    QStringList m_fileList, m_currentFileData;
    KNMusicGlobal::MusicDetailsInfo m_currentFileAppendData;
    bool m_working=false, m_noUpdating=true;
};

#endif // KNMUSICINFOCOLLECTORMANAGER_H
