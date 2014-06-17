#ifndef KNMUSICPLAYLISTSONGS_H
#define KNMUSICPLAYLISTSONGS_H

#include <QTreeView>

class KNMusicPlaylistSongs : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistSongs(QWidget *parent = 0);

signals:
    void requireOpenUrl(const QModelIndex &index);

public slots:

private slots:
    void onItemActived(const QModelIndex &index);
};

#endif // KNMUSICPLAYLISTSONGS_H
