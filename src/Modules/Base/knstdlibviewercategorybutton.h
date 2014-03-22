#ifndef KNSTDLIBVIEWERCATEGORYBUTTON_H
#define KNSTDLIBVIEWERCATEGORYBUTTON_H

#include "knlibviewercategorybutton.h"

class QBoxLayout;
class QGraphicsOpacityEffect;
class QMouseEvent;
class QResizeEvent;
class QTimeLine;
class KNStdLibViewerCategoryButton : public KNLibViewerCategoryButton
{
    Q_OBJECT
public:
    explicit KNStdLibViewerCategoryButton(QWidget *parent = 0);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void onCheckedChanged();

private slots:
    void moveCaption(int frame);
    void setCaptionFontSize(int pointSize);
    void mouseInAnime(qreal value);
    void mouseOutAnime(qreal value);

private:
    const int animeDuration=300;
    const int textDuration=100;

    void showTextAnime();
    void hideTextAnime();
    void startMouseDownAnime();
    void startMouseUPAnime();

    QTimeLine *m_mouseInAnime, *m_mouseOutAnime,
              *m_mouseDownAnime, *m_mouseUpAnime;
    QGraphicsOpacityEffect *m_iconOpacity;
    QPalette m_pal;
    QColor m_backcolor=QColor(0x85,0x85,0x85,0);
    QFont m_captionFont;
    int m_defaultFontSize, m_pressedFontSize;
    bool m_mousePressed=false;
};

#endif // KNSTDLIBVIEWERCATEGORYBUTTON_H
