#ifndef KNSTDLIBCATEGORYBUTTON_H
#define KNSTDLIBCATEGORYBUTTON_H

#include <QWidget>

class QBoxLayout;
class QLabel;
class QTimeLine;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QGraphicsOpacityEffect;
class KNStdLibCategoryButton : public QWidget
{
    Q_OBJECT
public:
    explicit KNStdLibCategoryButton(QWidget *parent = 0);
    void setCategoryIcon(const QPixmap &pixmap);
    void setCategoryText(const QString &text);

signals:
    void requireResetLeftSpace(const int &leftMargin);
    void requireShowCategorySelect();
    void requireHideCategorySelect();

public slots:
    void onActionSwitchTo(const QPixmap &pixmap,
                          const QString &text);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

private slots:
    void onActionChangeOpacity(const int &frame);

private:
    void resizeButton();
    QLabel *m_text, *m_icon;
    int m_leftMargin=58, m_iconSize=110, m_offset=10, m_textY=22, m_textSize=50;
    QFont m_textFont;
    QGraphicsOpacityEffect *m_iconEffect, *m_textEffect;
    QRadialGradient m_iconGradient;
    QLinearGradient m_textGradient;
    QColor m_opacityColor=QColor(255,255,255,0);
    QPixmap m_switchToPixmap;
    QString m_switchToText;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPropertyAnimation *m_iconOut, *m_textOut, *m_iconIn, *m_textIn;
    QParallelAnimationGroup *m_outAnime, *m_inAnime;
    bool m_selectionFolded=true, m_isPressed=false;
};

#endif // KNSTDLIBCATEGORYBUTTON_H
