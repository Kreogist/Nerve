#include <QBuffer>

#include "knlibhashpixmaplist.h"

KNLibHashPixmapList::KNLibHashPixmapList(QObject *parent) :
    QObject(parent)
{
    m_buffer=new QBuffer(&m_pixmapCache, this);
}

QPixmap KNLibHashPixmapList::pixmap(const QByteArray &array) const
{
    return m_list[array];
}
