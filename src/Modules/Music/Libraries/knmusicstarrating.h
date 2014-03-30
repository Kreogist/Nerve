#ifndef KNMUSICSTARRATING_H
#define KNMUSICSTARRATING_H

#include <QMetaType>
#include <QPointF>
#include <QVector>
#include <QPainter>

class KNMusicStarRating
{
public:
    KNMusicStarRating(int starCount = 0, int maxStarCount = 5);
    enum EditMode
    {
        Editable,
        ReadOnly
    };

    void paint(QPainter *painter,
               const QRect &rect,
               const QPalette &palette,
               EditMode mode) const;
    QSize sizeHint() const;
    int starCount() const
    {
        return m_starCount;
    }
    int maxStarCount() const
    {
        return m_maxStarCount;
    }
    void setStarCount(const int &starCount)
    {
        m_starCount = starCount;
    }
    void setMaxStarCount(const int &maxStarCount)
    {
        m_maxStarCount = maxStarCount;
    }

private:
    QPolygonF starPolygon;
    QPolygonF diamondPolygon;
    int m_starCount;
    int m_maxStarCount;
};

Q_DECLARE_METATYPE(KNMusicStarRating)

#endif // KNMUSICSTARRATING_H
