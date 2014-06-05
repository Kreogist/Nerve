#ifndef KNSTDLIBCATEGORYLISTBUTTON_H
#define KNSTDLIBCATEGORYLISTBUTTON_H

#include <QLabel>

class QTimeLine;
class KNStdLibCategoryListButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNStdLibCategoryListButton(QWidget *parent = 0);

signals:
    void clicked();

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void changeBackground(const int &frame);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor=QColor(255,255,255,0);
    bool m_isPressed=false;
};

#endif // KNSTDLIBCATEGORYLISTBUTTON_H
