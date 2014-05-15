#ifndef KNLIBHASHPIXMAPLIST_H
#define KNLIBHASHPIXMAPLIST_H

#include <QPixmap>
#include <QHash>
#include <QByteArray>
#include <QThread>
#include <QList>
#include <QObject>

class QBuffer;

class KNLibPixmapBuffer : public QObject
{
    Q_OBJECT
public:
    explicit KNLibPixmapBuffer(QObject *parent = 0);
    void savePixmap();
    QString hashData() const;
    void setFolderPath(const QString &folderPath);
    QPixmap pixmap() const;

signals:
    void cacheComplete();
    void saveComplete();
    void requireRecoverData(QString key,
                            QPixmap pixmap);

public slots:
    void cachePixmap(const QPixmap &pixmap);

private:
    QByteArray m_pixmapCache, m_hashCache, m_deepHashCache;
    QString m_hashData;
    QBuffer *m_buffer;
    QString m_folderPath;
    QPixmap m_pixmap;
};

class KNLibHashPixmapList : public QObject
{
    Q_OBJECT
public:
    explicit KNLibHashPixmapList(QObject *parent = 0);
    ~KNLibHashPixmapList();
    QPixmap pixmap(const QString &key) const;
    void append(const int rowIndex, const QPixmap pixmap);
    void removeCurrentUpdate();
    void setAlbumArtPath(const QString &path);
    int currentRow() const;
    QString currentKey() const;

signals:
    void requireUpdatePixmap();
    void requireCachePixmap(const QPixmap &pixmap);

public slots:

private slots:
    void onActionCacheComplete();
    void onActionSaveComplete();
    void onActionRecoverData(const QString &key,
                             const QPixmap &pixmap);

private:
    struct AnalysisQueueItem
    {
        int row;
        QPixmap pixmap;
    };
    struct UpdateQueueItem
    {
        int row;
        QString key;
    };

    QList<AnalysisQueueItem> m_analysisQueue;
    QList<UpdateQueueItem> m_updateQueue;
    QHash<QString, QPixmap> m_list;
    KNLibPixmapBuffer *m_buffer;
    bool m_working=false;
    QString m_folderPath;
    QThread m_bufferThread;
};

#endif // KNLIBHASHPIXMAPLIST_H
