#ifndef KNMUSICPLAYLISTSONGS_H
#define KNMUSICPLAYLISTSONGS_H

#include "knmusiclistviewbase.h"

class QTimeLine;
class KNMusicPlaylistHeader;
class KNMusicPlaylistSongs : public KNMusicListViewBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistSongs(QWidget *parent = 0);
    void resetHeader();
    void resetIndexColumn();

signals:

public slots:

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void changeBackground(int frameData);
    void onItemActived(const QModelIndex &index);

private:
    KNMusicPlaylistHeader *m_headerWidget;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QColor m_backgroundColor;
    QPalette m_palette;
};

#endif // KNMUSICPLAYLISTSONGS_H
