#ifndef KNSTDLIBCATEGORYBUTTON_H
#define KNSTDLIBCATEGORYBUTTON_H

#include <QLabel>

class QBoxLayout;
class QGraphicsOpacityEffect;
class KNStdLibCategoryButton : public QLabel
{
    Q_OBJECT
public:
    explicit KNStdLibCategoryButton(QWidget *parent = 0);
    void setCategoryIcon(const QPixmap &pixmap);
    void setCategoryText(const QString &text);

signals:
    void requireResetLeftSpace(const int &leftMargin);

private:
    void resizeButton();
    QLabel *m_text, *m_icon;
    int m_leftMargin=50, m_iconSize=110;
    QGraphicsOpacityEffect *m_iconEffect, *m_textEffect;
    QRadialGradient m_iconGradient;
};

#endif // KNSTDLIBCATEGORYBUTTON_H
