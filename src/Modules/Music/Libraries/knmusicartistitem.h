#ifndef KNMUSICARTISTITEM_H
#define KNMUSICARTISTITEM_H

#include <QList>
#include <QModelIndex>

#include <QStandardItem>

class KNMusicArtistItem : public QStandardItem
{
public:
    explicit KNMusicArtistItem(QString name = "");
    void addSongs(const QModelIndex &index);

    QList<QModelIndex> songs() const;
    void setSongs(const QList<QModelIndex> &songs);

signals:

public slots:

private:
    QList<QModelIndex> m_songs;
};

#endif // KNMUSICARTISTITEM_H
