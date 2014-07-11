#ifndef KNSTDLIBVIEWERCATEGORYBUTTON_H
#define KNSTDLIBVIEWERCATEGORYBUTTON_H

#include "knlibviewercategorybutton.h"

class QBoxLayout;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;
class QTimeLine;
class KNStdLibViewerCategoryButton : public KNLibViewerCategoryButton
{
    Q_OBJECT
public:
    explicit KNStdLibViewerCategoryButton(QWidget *parent = 0);
    QPixmap icon() const;
    QString text() const;
    void setIcon(const QPixmap &icon);
    void setText(const QString &text);

signals:

public slots:

protected:
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:

private:
    QPixmap m_icon;
    QString m_text;
    bool m_mousePressed=false;
};

#endif // KNSTDLIBVIEWERCATEGORYBUTTON_H
