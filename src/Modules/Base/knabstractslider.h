#ifndef KNABSTRACTSLIDER_H
#define KNABSTRACTSLIDER_H

#include <QWidget>

class QMouseEvent;
class KNAbstractSlider : public QWidget
{
    Q_OBJECT
public:
    explicit KNAbstractSlider(QWidget *parent = 0);
    float maximum() const;
    float minimal() const;
    void setMaximum(float maximum);
    void setMinimum(float minimal);
    virtual void setValue(float value);
    void setRange(float min, float max);
    float value() const;
    bool isSliderDown() const;
    void setIsSliderDown(bool isSliderDown);

signals:
    void rangeChanged(float min, float max);
    void sliderMoved(float value);
    void sliderPressed();
    void sliderReleased();
    void valueChanged(int value);

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    float m_range=100.0;
    float m_value=0.0;
    float m_minimal=0.0;
    float m_maximum=100.0;
    bool m_isSliderDown=false;

private:
};

#endif // KNABSTRACTSLIDER_H
