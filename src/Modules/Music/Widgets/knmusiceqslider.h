#ifndef KNMUSICEQSLIDER_H
#define KNMUSICEQSLIDER_H

#include <QPixmap>

#include "../../Base/knabstractvslider.h"

class KNMusicEQSlider : public KNAbstractVSlider
{
    Q_OBJECT
public:
    explicit KNMusicEQSlider(QWidget *parent = 0);
    void setValue(float value);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void updateButtonPosition();

private:
    QPixmap m_sliderBase, m_sliderButton;
    float m_topRange, m_topMargin, m_bottomMargin, m_buttonTop, m_mouseRange;
};

#endif // KNMUSICEQSLIDER_H
