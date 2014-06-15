#ifndef KNMUSICABSTRACTPLAYLIST_H
#define KNMUSICABSTRACTPLAYLIST_H

#include <QObject>

class KNMusicAbstractPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicAbstractPlaylist(QObject *parent = 0);
    virtual QString nextSong();
    virtual QString prevSong();

signals:

public slots:

};

#endif // KNMUSICABSTRACTPLAYLIST_H
