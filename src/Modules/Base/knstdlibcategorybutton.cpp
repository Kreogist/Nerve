#include <QLinearGradient>
#include <QGraphicsOpacityEffect>

#include "knstdlibcategorybutton.h"

KNStdLibCategoryButton::KNStdLibCategoryButton(QWidget *parent) :
    QLabel(parent)
{
    setContentsMargins(0,0,0,0);
    QPalette pal=palette();
    pal.setColor(QPalette::WindowText, QColor(255,255,255));
    setPalette(pal);
    m_text=new QLabel(this);
    m_text->move(m_leftMargin, 22);
    QFont textFont=m_text->font();
    textFont.setPixelSize(50);
    textFont.setBold(true);
    m_text->setFont(textFont);
    m_textEffect=new QGraphicsOpacityEffect(m_text);
    m_textEffect->setOpacity(1);
    QLinearGradient textGradient=QLinearGradient(QPointF(0,0),
                                                 QPointF(0,50));
    textGradient.setColorAt(0, Qt::black);
    textGradient.setColorAt(1, Qt::transparent);
    m_textEffect->setOpacityMask(textGradient);
    m_text->setGraphicsEffect(m_textEffect);
    m_icon=new QLabel(this);
    m_icon->setScaledContents(true);
    m_icon->setFixedSize(m_iconSize, m_iconSize);
    m_iconEffect=new QGraphicsOpacityEffect(m_icon);
    m_iconGradient=QRadialGradient(QPointF(0,0),
                                    m_iconSize,
                                    QPointF(0,0));
    m_iconGradient.setColorAt(0.0, Qt::black);
    m_iconGradient.setColorAt(1.0, Qt::transparent);
    m_iconEffect->setOpacityMask(m_iconGradient);
    m_icon->setGraphicsEffect(m_iconEffect);
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

void KNStdLibCategoryButton::resizeButton()
{
    int sizeParam=qMax(m_text->fontMetrics().width(m_text->text())+m_leftMargin,
                       m_iconSize);
    setFixedWidth(sizeParam);
    emit requireResetLeftSpace(sizeParam);
}
