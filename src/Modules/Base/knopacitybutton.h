#ifndef KNOPACITYBUTTON_H
#define KNOPACITYBUTTON_H

#include <QLabel>

class QPropertyAnimation;
class QGraphicsOpacityEffect;
class KNOpacityButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNOpacityButton(QWidget *parent = 0);

signals:
    void clicked();
    void mousePressed();
    void mouseReleased();

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QGraphicsOpacityEffect *m_effect;
    QPropertyAnimation *m_mouseDown, *m_mouseUp, *m_mouseIn, *m_mouseOut;
    bool m_isPressed=false;
};

#endif // KNOPACITYBUTTON_H
