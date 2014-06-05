#ifndef KNMUSICVOLUMESLIDER_H
#define KNMUSICVOLUMESLIDER_H

#include "../../Base/knabstractslider.h"

class KNMusicVolumeSlider : public KNAbstractSlider
{
    Q_OBJECT
public:
    explicit KNMusicVolumeSlider(QWidget *parent = 0);
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
    QPixmap m_sliderBase, m_sliderButton, m_sliderBaseLeft, m_sliderBaseRight,
            m_kopieLeft, m_kopieMid, m_kopieRight;
    float m_leftRange, m_leftMargin, m_rightMargin, m_buttonLeft, m_mouseRange,
          m_leftKopieStart=4, m_leftKopieMargin;
};

#endif // KNMUSICVOLUMESLIDER_H
