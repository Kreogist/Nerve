#ifndef KNSTDLIBVIEWERCATEGORYBUTTON_H
#define KNSTDLIBVIEWERCATEGORYBUTTON_H

#include <QLinearGradient>

#include "knlibviewercategorybutton.h"

class QBoxLayout;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;
class QTimeLine;
class KNStdLibViewerCategoryButton : public KNLibViewerCategoryButton
{
    Q_OBJECT
public:
    explicit KNStdLibViewerCategoryButton(QWidget *parent = 0);
    QPixmap icon() const;
    QString text() const;
    void setIcon(const QPixmap &icon);
    void setText(const QString &text);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void onCheckedChanged();

private slots:
    void onActionMouseInOutFrameChange(const int &frame);
    void onActionMousePressedChange(const int &frame);

private:
    void startMouseOutAnime();
    void startMouseUpAnime();
    QPixmap m_icon;
    QString m_text;
    QLinearGradient m_background;
    int m_iconSize=20, m_fixedHeight=30, m_iconY=5, m_iconCenterX=0, m_textY=-95;
    qreal m_iconAlpha=1.0, m_textAlpha=0.0;
    QColor m_highLightColor=QColor(255,255,255,0),
           m_gradientColor=QColor(255,255,255,0);
    QTimeLine *m_mouseIn, *m_mouseOut, *m_mouseUp, *m_mouseDown;
    bool m_mousePressed=false;
};

#endif // KNSTDLIBVIEWERCATEGORYBUTTON_H
