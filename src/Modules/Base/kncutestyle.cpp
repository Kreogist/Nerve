#include <QStyleFactory>

#include "kncutestyle.h"

KNCuteStyle::KNCuteStyle() :
    QCommonStyle()
{
    m_fusionStyle=QStyleFactory::create("fusion");
    m_fusionStyle->setParent(this);
}

void KNCuteStyle::drawControl(QStyle::ControlElement element,
                              const QStyleOption *opt,
                              QPainter *p,
                              const QWidget *w) const
{
    m_fusionStyle->drawControl(element,opt,p,w);
}

void KNCuteStyle::drawPrimitive(QStyle::PrimitiveElement pe,
                                const QStyleOption *opt,
                                QPainter *p,
                                const QWidget *w) const
{
    m_fusionStyle->drawPrimitive(pe, opt, p, w);
}

void KNCuteStyle::drawComplexControl(QStyle::ComplexControl cc,
                                     const QStyleOptionComplex *opt,
                                     QPainter *p,
                                     const QWidget *w) const
{
    m_fusionStyle->drawComplexControl(cc, opt, p, w);
}

QSize KNCuteStyle::sizeFromContents(QStyle::ContentsType ct,
                                    const QStyleOption *opt,
                                    const QSize &contentsSize,
                                    const QWidget *widget) const
{
    return m_fusionStyle->sizeFromContents(ct, opt, contentsSize, widget);
}

int KNCuteStyle::pixelMetric(QStyle::PixelMetric metric,
                             const QStyleOption *option,
                             const QWidget *widget) const
{
    return m_fusionStyle->pixelMetric(metric,
                                      option,
                                      widget);
}

QRect KNCuteStyle::subElementRect(QStyle::SubElement r,
                                  const QStyleOption *opt,
                                  const QWidget *widget) const
{
    return m_fusionStyle->subElementRect(r, opt, widget);
}

QStyle::SubControl KNCuteStyle::hitTestComplexControl(QStyle::ComplexControl cc,
                                                      const QStyleOptionComplex *opt,
                                                      const QPoint &pt,
                                                      const QWidget *w) const
{
    return m_fusionStyle->hitTestComplexControl(cc, opt, pt, w);
}

QRect KNCuteStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    return m_fusionStyle->subControlRect(cc, opt, sc, widget);
}

QPixmap KNCuteStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const
{
    return m_fusionStyle->generatedIconPixmap(iconMode, pixmap, opt);
}

int KNCuteStyle::styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const
{
    return m_fusionStyle->styleHint(hint, option, widget, returnData);
}

QRect KNCuteStyle::itemPixmapRect(const QRect &r, int flags, const QPixmap &pixmap) const
{
    return m_fusionStyle->itemPixmapRect(r, flags, pixmap);
}

QIcon KNCuteStyle::standardIcon(QStyle::StandardPixmap standardIcon, const QStyleOption *option, const QWidget *widget) const
{
    return m_fusionStyle->standardIcon(standardIcon, option, widget);
}

QPixmap KNCuteStyle::standardPixmap(QStyle::StandardPixmap standardPixmap, const QStyleOption *opt, const QWidget *widget) const
{
    return m_fusionStyle->standardPixmap(standardPixmap, opt, widget);
}

void KNCuteStyle::drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const
{
    m_fusionStyle->drawItemPixmap(painter, rect, alignment, pixmap);
}

void KNCuteStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
    m_fusionStyle->drawItemText(painter, rect, flags, pal, enabled, text, textRole);
}

void KNCuteStyle::polish(QWidget *widget)
{
    m_fusionStyle->polish(widget);
}

void KNCuteStyle::polish(QApplication *app)
{
    m_fusionStyle->polish(app);
}

void KNCuteStyle::polish(QPalette &pal)
{
    m_fusionStyle->polish(pal);
}

void KNCuteStyle::unpolish(QWidget *widget)
{
    m_fusionStyle->unpolish(widget);
}

void KNCuteStyle::unpolish(QApplication *app)
{
    m_fusionStyle->unpolish(app);
}


