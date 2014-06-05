#ifndef KNSTDLIBCATEGORYSWITCHER_H
#define KNSTDLIBCATEGORYSWITCHER_H

#include "knlibcategoryswitcher.h"

class QBoxLayout;
class QResizeEvent;
class QTimeLine;
class KNVerticalWidgetSwitcher;
class KNStdLibCategorySwitcher : public KNLibCategorySwitcher
{
    Q_OBJECT
public:
    explicit KNStdLibCategorySwitcher(QWidget *parent = 0);

signals:

public slots:
    void addCategory(const QString &name,
                     const QPixmap &icon,
                     QWidget *centralWidget);
    void setCentralWidgetIndex(const int &index);
    void changeOpacity(const qreal &opacityParam);
    void enableContent();
    void disableContent();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QBoxLayout *m_layout;
    QWidget *m_dummyShadow;
    QTimeLine *m_disableContent, *m_enableContent;
    KNVerticalWidgetSwitcher *m_categoryWidgets;
    QColor m_coverColor;
    QPalette m_dummyPalette;
    QColor m_shadowColor=QColor(0,0,0,0);
};

#endif // KNSTDLIBCATEGORYSWITCHER_H
