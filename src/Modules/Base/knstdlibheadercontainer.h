#ifndef KNSTDLIBHEADERCONTAINER_H
#define KNSTDLIBHEADERCONTAINER_H

#include "knlibheadercontainer.h"

class QBoxLayout;
class QTimeLine;
class KNStdLibCategoryButton;
class KNStdLibHeaderContainer : public KNLibHeaderContainer
{
    Q_OBJECT
public:
    explicit KNStdLibHeaderContainer(QWidget *parent = 0);
    void addCategorySwitcher(KNStdLibCategoryButton *button);
    void addHeaderSwitcher(KNLibHeaderSwitcher *switcher);

signals:

public slots:

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void resetLeftSpace(const int &leftMargin);
    void changeBackground(int frameData);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor;
    QBoxLayout *m_containerLayout;
};

#endif // KNSTDLIBHEADERCONTAINER_H
