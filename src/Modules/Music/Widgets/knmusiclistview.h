#ifndef KNMUSICLISTVIEW_H
#define KNMUSICLISTVIEW_H

#include "knmusiclibrarylistview.h"

class QTimeLine;
class KNMusicListView : public KNMusicLibraryListview
{
    Q_OBJECT
public:
    explicit KNMusicListView(QWidget *parent = 0);
    void resetHeader();

signals:

public slots:

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void changeBackground(int frameData);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QColor m_backgroundColor;
    QPalette m_palette;
};

#endif // KNMUSICLISTVIEW_H
