#ifndef KNMUSICPLAYLISTVIEW_H
#define KNMUSICPLAYLISTVIEW_H

#include <QSplitter>

class QTreeView;
class QAbstractItemModel;
class KNMusicPlaylistManager;
class KNMusicPlaylistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistView(QWidget *parent = 0);
    void setManager(KNMusicPlaylistManager *manager);

signals:

public slots:

private:
    void setPlaylistList(QAbstractItemModel *model);
    KNMusicPlaylistManager *m_manager;
    QTreeView *m_playlistListView;
};

#endif // KNMUSICPLAYLISTVIEW_H
