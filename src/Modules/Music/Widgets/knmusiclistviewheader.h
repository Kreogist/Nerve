#ifndef KNMUSICLISTVIEWHEADER_H
#define KNMUSICLISTVIEWHEADER_H

#include <QLinearGradient>

#include "../knmusicglobal.h"

#include <QHeaderView>

class QTimeLine;
class KNMusicListViewHeaderMenu;
class KNMusicListViewHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit KNMusicListViewHeader(QWidget *parent = 0);
    void moveToFirst(int logicalIndex);
    void hideStrectch();
    void showStrectch();

signals:
    void requireChangeVisible(const int &index,
                              const bool &visible);

public slots:

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

private slots:
    void changeBackground(int frameData);
    void showContextMenu(const QPoint &mousePoint);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QColor m_backgroundColor, m_lineColor;
    QPalette m_palette;
    QLinearGradient buttonGradient;
    QPolygonF m_ascPoints, m_desPoints;
    KNMusicListViewHeaderMenu *m_viewerMenu;
};

#endif // KNMUSICLISTVIEWHEADER_H
