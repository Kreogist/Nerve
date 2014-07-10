#ifndef KNMUSICLISTVIEWHEADERBASE_H
#define KNMUSICLISTVIEWHEADERBASE_H

#include <QLinearGradient>

#include "../knmusicglobal.h"

#include <QHeaderView>

class QTimeLine;
class KNMusicListViewHeaderBase : public QHeaderView
{
    Q_OBJECT
public:
    explicit KNMusicListViewHeaderBase(QWidget *parent = 0);
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

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QColor m_backgroundColor, m_lineColor;
    QPalette m_palette;
    QLinearGradient m_buttonGradient;
};

#endif // KNMUSICLISTVIEWHEADERBASE_H
