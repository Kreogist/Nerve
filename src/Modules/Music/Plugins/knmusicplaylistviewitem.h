#ifndef KNMUSICPLAYLISTVIEWITEM_H
#define KNMUSICPLAYLISTVIEWITEM_H

#include "../Base/knmusicviewerplaylistitembase.h"

class KNMusicPlaylistView;
class KNMusicPlaylistViewItem : public KNMusicViewerPlaylistItemBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistViewItem(QObject *parent = 0);
    ~KNMusicPlaylistViewItem();
    void applyPlugin();

signals:

public slots:
    void retranslate();
    void setPlaylistManager(KNMusicPlaylistManagerBase *manager);

private:
    KNMusicPlaylistView *m_playlistView;
    QString m_title;
};

#endif // KNMUSICPLAYLISTVIEWITEM_H
