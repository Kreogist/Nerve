#ifndef KNMUSICPLAYLISTMANAGER_H
#define KNMUSICPLAYLISTMANAGER_H

#include <QList>
#include <QModelIndex>
#include <QObject>

class KNMusicPlaylistManager : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistManager(QObject *parent = 0);

signals:

public slots:

private:
    struct PlayList
    {
        QString name;
        QList<QModelIndex> songs;
    };
    QList<PlayList> m_playlists;
};

#endif // KNMUSICPLAYLISTMANAGER_H
