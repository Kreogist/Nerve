#ifndef KNMUSICPLAYLISTVIEWER_H
#define KNMUSICPLAYLISTVIEWER_H

#include <QTreeView>

class KNMusicPlaylistViewer : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistViewer(QWidget *parent = 0);
    void resetHeader();

signals:

public slots:

};

#endif // KNMUSICPLAYLISTVIEWER_H
