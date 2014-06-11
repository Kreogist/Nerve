#ifndef KNMUSICARTISTLIST_H
#define KNMUSICARTISTLIST_H

#include <QListView>

class QTimeLine;
class KNMusicCategoryModel;
class KNMusicCategoryList : public QListView
{
    Q_OBJECT
public:
    explicit KNMusicCategoryList(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void changeBackground(int frameData);

private:
    QTimeLine *m_mouseIn, *m_mouseOut;
    QPalette m_palette;
    QColor m_backgroundColor;
    int m_minGrey=0x30;
};

#endif // KNMUSICARTISTLIST_H
