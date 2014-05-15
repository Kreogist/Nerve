#ifndef KNLIBHASHPIXMAPLIST_H
#define KNLIBHASHPIXMAPLIST_H

#include <QPixmap>
#include <QHash>
#include <QByteArray>
#include <QObject>

class QBuffer;

class KNLibPixmapBuffer : public QObject
{
    Q_OBJECT
public:
    explicit KNLibPixmapBuffer(QObject *parent = 0);
    void cachePixmap(const QPixmap &pixmap);

signals:
    void cacheComplete();
    void writeComplete();

private:
    QByteArray m_pixmapCache, m_hashCache;
    QBuffer *m_buffer;
};

class KNLibHashPixmapList : public QObject
{
    Q_OBJECT
public:
    explicit KNLibHashPixmapList(QObject *parent = 0);
    QPixmap pixmap(const QByteArray &array) const;
    QByteArray append(const QPixmap pixmap);

signals:

public slots:

private:
    QHash<QByteArray, QPixmap> m_list;
};

#endif // KNLIBHASHPIXMAPLIST_H
