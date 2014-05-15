#ifndef KNLIBHASHPIXMAPLIST_H
#define KNLIBHASHPIXMAPLIST_H

#include <QPixmap>
#include <QHash>
#include <QByteArray>
#include <QObject>

class QBuffer;
class KNLibHashPixmapList : public QObject
{
    Q_OBJECT
public:
    explicit KNLibHashPixmapList(QObject *parent = 0);
    QPixmap pixmap(const QByteArray &array) const;
    ;

signals:

public slots:

private:
    QHash<QByteArray, QPixmap> m_list;
    QByteArray m_pixmapCache;
    QBuffer *m_buffer;
};

#endif // KNLIBHASHPIXMAPLIST_H
