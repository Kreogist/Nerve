#ifndef KNSTDLIBHEADERCONTAINER_H
#define KNSTDLIBHEADERCONTAINER_H

#include "knlibheadercontainer.h"

class QResizeEvent;
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
    void resizeEvent(QResizeEvent *event);

private slots:
    void resetLeftSpace(const int &leftMargin);
    void changeBackground(int frameData);

private:
    void linkButtonAndSwitcher();
    void resetSwitcherPosition();
    int m_leftMargin=0;
    QTimeLine *m_mouseIn, *m_mouseOut;
    KNStdLibCategoryButton *m_button=NULL;
    KNLibHeaderSwitcher *m_switcher=NULL;
    QPalette m_palette;
    QColor m_backgroundColor;
};

#endif // KNSTDLIBHEADERCONTAINER_H
