#include <QWidget>
#include <cmath>

#include "knmusicstarrating.h"

const int PaintingScaleFactor = 20;

KNMusicStarRating::KNMusicStarRating(int starCount, int maxStarCount)
{
    m_starCount = starCount;
    m_maxStarCount = maxStarCount;

    starPolygon << QPointF(1.0, 0.5);
    for (int i = 1; i < 5; ++i)
        starPolygon << QPointF(0.5 + 0.5 * cos(0.8 * i * 3.14),
                               0.5 + 0.5 * sin(0.8 * i * 3.14));

    diamondPolygon << QPointF(0.4, 0.5) << QPointF(0.5, 0.4)
                   << QPointF(0.6, 0.5) << QPointF(0.5, 0.6)
                   << QPointF(0.4, 0.5);
}

void KNMusicStarRating::paint(QPainter *painter,
                              const QRect &rect,
                              const QPalette &palette,
                              KNMusicStarRating::EditMode mode) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    if (mode == Editable)
    {
        painter->setBrush(QColor(255,255,255));
    }
    else
    {
        painter->setBrush(QColor(255,255,255));
    }

    int yOffset = (rect.height() - PaintingScaleFactor) / 2;
    painter->translate(rect.x(), rect.y() + yOffset);
    painter->scale(PaintingScaleFactor, PaintingScaleFactor);

    for (int i = 0; i < m_maxStarCount; ++i) {
        if (i < m_starCount) {
            painter->drawPolygon(starPolygon, Qt::WindingFill);
        } else if (mode == Editable) {
            painter->drawPolygon(diamondPolygon, Qt::WindingFill);
        }
        painter->translate(1.0, 0.0);
    }

    painter->restore();
}

QSize KNMusicStarRating::sizeHint() const
{
    return PaintingScaleFactor * QSize(m_maxStarCount, 1);
}
