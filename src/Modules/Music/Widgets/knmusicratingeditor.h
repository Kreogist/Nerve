#ifndef KNMUSICRATINGEDITOR_H
#define KNMUSICRATINGEDITOR_H

#include "../Libraries/knmusicstarrating.h"

#include <QWidget>

class KNMusicRatingEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicRatingEditor(QWidget *parent = 0);
    QSize sizeHint() const
    {
        return m_starRating.sizeHint();
    }
    void setStarRating(const KNMusicStarRating &starRating)
    {
        m_starRating = starRating;
    }
    KNMusicStarRating starRating()
    {
        return m_starRating;
    }

signals:
    void editingFinished();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int starAtPosition(int x);
    KNMusicStarRating m_starRating;

};

#endif // KNMUSICRATINGEDITOR_H
