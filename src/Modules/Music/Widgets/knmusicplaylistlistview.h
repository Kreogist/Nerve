#ifndef KNMUSICPLAYLISTLISTVIEW_H
#define KNMUSICPLAYLISTLISTVIEW_H

#include <QListView>

class QTimeLine;
class KNMusicPlaylistListview : public QListView
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListview(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void changeBackground(int frameData);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor;
    int m_minGrey=0x30;
};

#endif // KNMUSICPLAYLISTLISTVIEW_H
