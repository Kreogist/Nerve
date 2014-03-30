#include <QMouseEvent>

#include "../Libraries/knmusicstarrating.h"
#include "knmusicratingeditor.h"

KNMusicRatingEditor::KNMusicRatingEditor(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
}

void KNMusicRatingEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    m_starRating.paint(&painter, rect(), this->palette(),
                       KNMusicStarRating::Editable);
}

void KNMusicRatingEditor::mouseMoveEvent(QMouseEvent *event)
{
    int star = starAtPosition(event->x());

    if (star != m_starRating.starCount() && star != -1) {
        m_starRating.setStarCount(star);
        update();
    }
}

void KNMusicRatingEditor::mouseReleaseEvent(QMouseEvent *event)
{
    emit editingFinished();
}

int KNMusicRatingEditor::starAtPosition(int x)
{
    int star = (x / (m_starRating.sizeHint().width()
                     / m_starRating.maxStarCount())) + 1;
    if (star <= 0 || star > m_starRating.maxStarCount())
        return -1;

    return star;
}


