#ifndef KNMUSICINFOCOLLECTORMANAGER_H
#define KNMUSICINFOCOLLECTORMANAGER_H

#include <QThread>
#include <QList>
#include <QModelIndex>

#include "../Base/knmusicinfocollectormanagerbase.h"

class QStandardItem;
class KNMusicInfoCollector;
class KNMusicInfoCollectorManager : public KNMusicInfoCollectorManagerBase
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollectorManager(QObject *parent = 0);
    ~KNMusicInfoCollectorManager();

    QStringList currentFileData() const;
    KNMusicGlobal::MusicDetailsInfo currentFileAppendData() const;
    QStandardItem *currentIndex() const;
    void removeFirstUpdateResult();
    bool isUpdateQueueEmpty();
    bool isWorking();
    void setMusicBackend(KNMusicBackend *backend);

signals:

public slots:
    void addAnalysisList(QStandardItem *index, QString filePath);
    void removedIndexesInList(QModelIndex removedIndex);

private slots:
    void analysisNext();
    void currentWorkDone(QString filePath,
                         QStringList value,
                         KNMusicGlobal::MusicDetailsInfo datas);

private:
    struct AnalysisQueueItem
    {
        QStandardItem *index;
        QString filePath;
    };
    struct ResultQueueItem
    {
        QStandardItem *index;
        QStringList text;
        KNMusicGlobal::MusicDetailsInfo details;
    };

    KNMusicInfoCollector *m_collector;
    QThread m_collectThread;
    QList<AnalysisQueueItem> m_analysisQueue;
    QList<ResultQueueItem> m_resultQueue;
    bool m_working=false, m_noUpdating=true;
};

#endif // KNMUSICINFOCOLLECTORMANAGER_H
