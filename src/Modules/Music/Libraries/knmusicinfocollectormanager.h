#ifndef KNMUSICINFOCOLLECTORMANAGER_H
#define KNMUSICINFOCOLLECTORMANAGER_H

#include <QThread>
#include <QStringList>

#include <QObject>

class KNMusicInfoCollector;
class KNMusicInfoCollectorManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicInfoCollectorManager(QObject *parent = 0);
    ~KNMusicInfoCollectorManager();

signals:
    void requireAppendMusic(const QStringList &value,
                            const QPixmap &coverImage);

public slots:
    void addAnalysisList(const QString &filePath);

private slots:
    void currentWorkDone(const QStringList &value,
                         const QPixmap &coverImage);

private:
    KNMusicInfoCollector *m_collector;
    QThread m_collectThread;
    QStringList m_fileList;
    bool m_working=false;
};

#endif // KNMUSICINFOCOLLECTORMANAGER_H
