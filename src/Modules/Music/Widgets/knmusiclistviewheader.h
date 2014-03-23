#ifndef KNMUSICLISTVIEWHEADER_H
#define KNMUSICLISTVIEWHEADER_H

#include "../knmusicglobal.h"

#include <QHeaderView>

class QTimeLine;
class KNMusicListViewHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit KNMusicListViewHeader(QWidget *parent = 0);
    void moveToFirst(int logicalIndex);

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

#endif // KNMUSICLISTVIEWHEADER_H
