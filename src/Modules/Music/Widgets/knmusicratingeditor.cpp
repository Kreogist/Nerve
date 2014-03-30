#include <QDebug>

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
    QWidget::paintEvent(event);
    QPainter painter(this);
    m_starRating.paint(&painter, rect(), this->palette(),
                       KNMusicStarRating::Editable);
}

void KNMusicRatingEditor::mouseMoveEvent(QMouseEvent *event)
{
    int star = starAtPosition(event->x());

    if (star != m_starRating.starCount() && star != -1)
    {
        m_starRating.setStarCount(star);
        update();
    }
}

void KNMusicRatingEditor::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    emit editingFinished();
}

int KNMusicRatingEditor::starAtPosition(int x)
{
    int starNum=x<0?0:
                (x/(m_starRating.sizeHint().width()/m_starRating.maxStarCount())+1);
    if (starNum < 0 || starNum > m_starRating.maxStarCount())
    {
        return -1;
    }
    return starNum;
}


