#ifndef KNPLAYERPROGRESS_H
#define KNPLAYERPROGRESS_H

#include <QAbstractSlider>

class QTimeLine;
class QEvent;
class QPaintEvent;
class KNPlayerProgress : public QAbstractSlider
{
    Q_OBJECT
public:
    explicit KNPlayerProgress(QWidget *parent = 0);
    const QPalette &palette() const;
    void setPalette(const QPalette &pal);
signals:

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void onActionChangeColor(const int &time);

private:
    QPalette m_palette;
    QColor m_button, m_window;
    QTimeLine *m_mouseIn, *m_mouseOut;
    int m_scrollHeight=8;
};

#endif // KNPLAYERPROGRESS_H
