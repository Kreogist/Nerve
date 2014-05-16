#ifndef KNLIBHASHPIXMAPLIST_H
#define KNLIBHASHPIXMAPLIST_H

#include <QImage>
#include <QHash>
#include <QByteArray>
#include <QThread>
#include <QList>
#include <QObject>

class KNLibImageBuffer : public QObject
{
    Q_OBJECT
public:
    explicit KNLibImageBuffer(QObject *parent = 0);
    void setAlbumArtPath(const QString &folderPath);
    QString albumArtPath() const;
    QString hash() const;

signals:
    void hashComplete();
    void saveComplete();

public slots:
    void hashImage(const QImage &image);
    void saveImage();

private:
    QByteArray m_imageByteCache, m_hashResult;
    QString m_albumArtFolder, m_hash;
    QImage m_imageCache;
};

class KNLibHashPixmapList : public QObject
{
    Q_OBJECT
public:
    explicit KNLibHashPixmapList(QObject *parent = 0);
    ~KNLibHashPixmapList();
    QImage pixmap(const QString &key) const;
    void append(const int rowIndex, const QImage pixmap);
    void removeCurrentUpdate();
    void setAlbumArtPath(const QString &path);
    int currentRow() const;
    QString currentKey() const;

signals:
    void requireCacheImage(QImage pixmap);
    void requireUpdatePixmap();
    void requireSaveImage();
    void loadComplete();

public slots:
    void loadImages();

private slots:
    void onActionHashComplete();
    void onActionSaveComplete();
    void onActionRecoverData(const QString &key,
                             const QImage &pixmap);

private:
    struct AnalysisQueueItem
    {
        int row;
        QImage pixmap;
    };
    struct UpdateQueueItem
    {
        int row;
        QString key;
    };

    QList<AnalysisQueueItem> m_analysisQueue;
    QList<UpdateQueueItem> m_updateQueue;
    QHash<QString, QImage> m_list;
    KNLibImageBuffer *m_buffer;
    bool m_working=false, m_needToSaveImage=false;
    QString m_folderPath;
    QThread m_bufferThread;
};

#endif // KNLIBHASHPIXMAPLIST_H
