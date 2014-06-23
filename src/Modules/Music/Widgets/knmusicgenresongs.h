#ifndef KNMUSICGENRESONGS_H
#define KNMUSICGENRESONGS_H

#include "knmusiclibrarylistview.h"

class QTimeLine;
class KNMusicGenreSongs : public KNMusicLibraryListview
{
    Q_OBJECT
public:
    explicit KNMusicGenreSongs(QWidget *parent = 0);
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
    QColor m_alternateColor;
    QPalette m_palette;
};

#endif // KNMUSICGENRESONGS_H
