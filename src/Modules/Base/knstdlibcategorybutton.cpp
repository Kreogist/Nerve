#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QTimeLine>

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
    m_iconEffect->setOpacity(0.7);
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

void KNStdLibCategoryButton::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_mouseOut->stop();
    m_mouseIn->setStartFrame(m_opacityColor.alpha());
    m_mouseIn->start();
}

void KNStdLibCategoryButton::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_mouseIn->stop();
    m_mouseOut->setStartFrame(m_opacityColor.alpha());
    m_mouseOut->start();
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
    int sizeParam=qMax(m_text->fontMetrics().width(m_text->text())+m_leftMargin,
                       m_iconSize);
    setFixedWidth(sizeParam);
    emit requireResetLeftSpace(sizeParam-m_offset);
}
