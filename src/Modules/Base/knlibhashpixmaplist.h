#ifndef KNLIBHASHPIXMAPLIST_H
#define KNLIBHASHPIXMAPLIST_H

#include <QImage>
#include <QModelIndex>
#include <QHash>
#include <QByteArray>
#include <QThread>
#include <QList>
#include <QObject>

class QStandardItem;
class KNLibImageBuffer : public QObject
{
    Q_OBJECT
public:
    explicit KNLibImageBuffer(QObject *parent = 0);
    void setAlbumArtPath(const QString &folderPath);
    QString albumArtPath() const;
    QString identifyData() const;
    QString hash() const;

signals:
    void hashComplete();
    void saveComplete();

public slots:
    void hashImage(QString identifyData, const QImage &image);
    void saveImage();

private:
    QByteArray m_imageByteCache, m_hashResult;
    QString m_albumArtFolder, m_hash, m_identifyData;
    QImage m_imageCache;
};

class KNLibHashPixmapList : public QObject
{
    Q_OBJECT
public:
    explicit KNLibHashPixmapList(QObject *parent = 0);
    ~KNLibHashPixmapList();
    QImage pixmap(const QString &key) const;
    void appendImage(QStandardItem *rowIndex,
                     const QImage pixmap);
    bool removeImage(const QString &key);
    void removeCurrentUpdate();
    void setAlbumArtPath(const QString &path);
    int currentRow() const;
    QString currentKey() const;
    int identifyDataRole() const;
    void setIdentifyDataRole(int identifyDataRole);

signals:
    void requireCacheImage(QString identifyData, QImage pixmap);
    void requireUpdatePixmap();
    void requireSaveImage();
    void loadComplete();

public slots:
    void removedIndexesInList(QModelIndex removedIndex);
    void loadImages();

private slots:
    void onActionHashComplete();
    void onActionSaveComplete();
    void onActionRecoverData(const QString &key,
                             const QImage &pixmap);

private:
    bool removeImageFile(const QString &key);
    struct AnalysisQueueItem
    {
        QStandardItem *index;
        QImage pixmap;
    };
    struct UpdateQueueItem
    {
        QStandardItem *index;
        QString key;
    };

    QList<AnalysisQueueItem> m_analysisQueue;
    QList<UpdateQueueItem> m_updateQueue;
    QHash<QString, QImage> m_list;
    KNLibImageBuffer *m_buffer;
    bool m_working=false, m_needToSaveImage=false;
    int m_identifyDataRole=Qt::UserRole;
    QString m_folderPath;
    QThread m_bufferThread;
};

#endif // KNLIBHASHPIXMAPLIST_H
