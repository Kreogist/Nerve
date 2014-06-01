#ifndef KNMUSICEQSLIDER_H
#define KNMUSICEQSLIDER_H

#include <QPixmap>

#include "../../Base/knabstractslider.h"

class KNMusicEQSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    explicit KNMusicEQSlider(QWidget *parent = 0);
    void setValue(float value);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void updateButtonPosition();

private:
    void setValueFromMouseParam(float mouseParam);
    QPixmap m_sliderBase, m_sliderButton, m_sliderBaseTop, m_sliderBaseBottom;
    float m_topRange, m_topMargin, m_bottomMargin, m_buttonTop, m_mouseRange;
};

#endif // KNMUSICEQSLIDER_H
