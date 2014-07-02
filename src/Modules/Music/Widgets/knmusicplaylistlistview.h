#ifndef KNMUSICPLAYLISTLISTVIEW_H
#define KNMUSICPLAYLISTLISTVIEW_H

#include "../Base/knmusicplaylistlistviewbase.h"

class QTimeLine;
class KNMusicPlaylistListView : public KNMusicPlaylistListViewBase
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListView(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

public slots:

private slots:
    void changeBackground(int frameData);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor;
    int m_minGrey=0x30;
};

#endif // KNMUSICPLAYLISTLISTVIEW_H
