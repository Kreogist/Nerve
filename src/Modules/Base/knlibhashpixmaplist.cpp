#include <QCryptographicHash>
#include <QBuffer>

#include "knlibhashpixmaplist.h"

KNLibPixmapBuffer::KNLibPixmapBuffer(QObject *parent) :
    QObject(parent)
{
    m_buffer=new QBuffer(&m_pixmapCache, this);
}

void KNLibPixmapBuffer::cachePixmap(const QPixmap &pixmap)
{
    pixmap.save(m_buffer, "png");
}

KNLibHashPixmapList::KNLibHashPixmapList(QObject *parent) :
    QObject(parent)
{
}

QPixmap KNLibHashPixmapList::pixmap(const QByteArray &array) const
{
    return m_list[array];
}

QByteArray KNLibHashPixmapList::append(const QPixmap pixmap)
{
    ;
}
