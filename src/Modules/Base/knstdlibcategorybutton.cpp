#include <QGraphicsOpacityEffect>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QLabel>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QTimeLine>

#include <QDebug>

#include "knstdlibcategorybutton.h"

KNStdLibCategoryButton::KNStdLibCategoryButton(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(0,0,0,0);
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(0xff,0xff,0xff));
    setPalette(pal);
    //Initial Icon
    m_icon=new QLabel(this);
    m_icon->setScaledContents(true);
    m_icon->setFixedSize(m_iconSize, m_iconSize);
    m_iconEffect=new QGraphicsOpacityEffect(m_icon);
    m_iconEffect->setOpacity(1.0);
    m_iconGradient=QRadialGradient(QPointF(0,0),
                                    m_iconSize,
                                    QPointF(0,0));
    m_iconGradient.setColorAt(0.0, Qt::black);
    m_iconGradient.setColorAt(1.0, m_opacityColor);
    m_iconEffect->setOpacityMask(m_iconGradient);
    m_icon->setGraphicsEffect(m_iconEffect);

    //Initial Text
    m_text=new QLabel(this);
    m_text->move(m_leftMargin, m_textY);
    m_textFont=m_text->font();
    m_textFont.setBold(true);
    m_textFont.setPixelSize(m_textSize);
    m_text->setFont(m_textFont);
    m_rawFontMetrics=m_text->fontMetrics();
    m_textEffect=new QGraphicsOpacityEffect(m_text);
    m_textEffect->setOpacity(0.5);
    m_textGradient=QLinearGradient(QPointF(0,0),
                                   QPointF(0,50));
    m_textGradient.setColorAt(0, Qt::black);
    m_textGradient.setColorAt(1, m_opacityColor);
    m_textEffect->setOpacityMask(m_textGradient);
    m_text->setGraphicsEffect(m_textEffect);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setEndFrame(255);
    m_mouseIn->setUpdateInterval(5);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryButton::onActionChangeOpacity);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(0);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNStdLibCategoryButton::onActionChangeOpacity);

    //Out moving animation.
    m_outAnime=new QParallelAnimationGroup(this);
    m_iconOut=new QPropertyAnimation(m_icon, "geometry", this);
    m_iconOut->setEasingCurve(QEasingCurve::OutCubic);
    m_outAnime->addAnimation(m_iconOut);
    m_textOut=new QPropertyAnimation(m_text, "geometry", this);
    m_textOut->setEasingCurve(QEasingCurve::OutCubic);
    m_outAnime->addAnimation(m_textOut);

    //In moving animation.
    m_inAnime=new QParallelAnimationGroup(this);
    m_iconIn=new QPropertyAnimation(m_icon, "geometry", this);
    m_iconIn->setEasingCurve(QEasingCurve::OutCubic);
    m_inAnime->addAnimation(m_iconIn);
    m_textIn=new QPropertyAnimation(m_text, "geometry", this);
    m_textIn->setEasingCurve(QEasingCurve::OutCubic);
    m_inAnime->addAnimation(m_textIn);

    connect(m_outAnime, &QParallelAnimationGroup::finished,
            [=]
            {
                // Prepare for the contents.
                setCategoryIcon(m_switchToPixmap);
                setCategoryText(m_switchToText);

                //Start the in animation.
                m_inAnime->start();
            });

    connect(m_inAnime, &QParallelAnimationGroup::finished,
            [=]
            {
                m_selectionFolded=true;
                if(!rect().contains(mapFromGlobal(QCursor::pos())))
                {
                    toNormalMode();
                }
                emit requireEnableHeader();
            });
}

void KNStdLibCategoryButton::setCategoryIcon(const QPixmap &pixmap)
{
    m_icon->setPixmap(pixmap);
}

void KNStdLibCategoryButton::setCategoryText(const QString &text)
{
    m_text->setText(text);
    resizeButton();
}

void KNStdLibCategoryButton::onActionSwitchTo(const QPixmap &pixmap,
                                              const QString &text)
{
    m_switchToPixmap=pixmap;
    m_switchToText=text;
    QRect iconCurrentPosition=m_icon->geometry(),
          textCurrentPosition=m_text->geometry(),
          iconOutPosition=QRect(m_icon->x(),
                                -m_icon->height(),
                                m_icon->width(),
                                m_icon->height()),
          textOutPosition=QRect(-m_text->width(),
                                m_text->y(),
                                m_text->width(),
                                m_text->height());
    //Set the out-animation parameters.
    m_iconOut->setStartValue(iconCurrentPosition);
    m_iconOut->setEndValue(iconOutPosition);
    m_textOut->setStartValue(textCurrentPosition);
    m_textOut->setEndValue(textOutPosition);
    //Set the in-animation parameters.
    m_iconIn->setStartValue(iconOutPosition);
    m_iconIn->setEndValue(iconCurrentPosition);
    m_textIn->setStartValue(textOutPosition);
    m_textIn->setEndValue(textCurrentPosition);
    m_outAnime->start();
}

void KNStdLibCategoryButton::toNormalMode()
{
    m_mouseIn->stop();
    m_mouseOut->setStartFrame(m_opacityColor.alpha());
    m_mouseOut->start();
}

void KNStdLibCategoryButton::toFoldMode()
{
    m_mouseOut->stop();
    m_mouseIn->setStartFrame(m_opacityColor.alpha());
    m_mouseIn->start();
}

void KNStdLibCategoryButton::toNormalModeAndSelectionFolded()
{
    m_selectionFolded=true;
    emit requireEnableHeader();
    toNormalMode();
}

void KNStdLibCategoryButton::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_isPressed=true;
}

void KNStdLibCategoryButton::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if(m_mouseOut->state()==QTimeLine::Running)
    {
        toFoldMode();
    }
}

void KNStdLibCategoryButton::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if(m_isPressed)
    {
        m_isPressed=false;
        if(m_selectionFolded)
        {
            m_selectionFolded=false;
            emit requireShowCategorySelect();
            emit requireDisableHeader();
            return;
        }
        m_selectionFolded=true;
        emit requireHideCategorySelect();
        emit requireEnableHeader();
    }
}

void KNStdLibCategoryButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    if(m_selectionFolded)
    {
        toFoldMode();
    }
}

void KNStdLibCategoryButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    if(m_selectionFolded)
    {
        toNormalMode();
    }
}

void KNStdLibCategoryButton::onActionChangeOpacity(const int &frame)
{
    m_opacityColor.setAlpha(frame);
    m_textGradient.setColorAt(1, m_opacityColor);
    m_iconGradient.setColorAt(1, m_opacityColor);
    m_textEffect->setOpacity((float)frame/510.0+0.5);
    m_textEffect->setOpacityMask(m_textGradient);
    m_iconEffect->setOpacityMask(m_iconGradient);
    m_icon->setFixedSize(m_iconSize-(frame>>3),
                         m_iconSize-(frame>>3));
    m_text->move(m_leftMargin+(frame>>3), m_textY-(frame>>4));
    m_textFont.setPixelSize(m_textSize-(frame>>3));
    m_text->setFont(m_textFont);
}

void KNStdLibCategoryButton::resizeButton()
{
    int sizeParam=qMax(m_rawFontMetrics.width(m_text->text())+m_leftMargin,
                       m_iconSize);
    setFixedWidth(sizeParam);
    emit requireResetLeftSpace(sizeParam-m_offset);
}
