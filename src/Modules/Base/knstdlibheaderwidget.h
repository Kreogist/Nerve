#ifndef KNSTDLIBHEADERWIDGET_H
#define KNSTDLIBHEADERWIDGET_H

#include "knlibheaderwidget.h"

class QTimeLine;
class KNStdLibHeaderWidget : public KNLibHeaderWidget
{
    Q_OBJECT
public:
    explicit KNStdLibHeaderWidget(QWidget *parent = 0);

signals:

public slots:

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void changeBackground(int frameData);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor;
};

#endif // KNSTDLIBHEADERWIDGET_H
