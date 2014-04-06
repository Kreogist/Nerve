#ifndef KNMUSICLISTVIEW_H
#define KNMUSICLISTVIEW_H

#include <QTreeView>

class QTimer;
class QTimeLine;
class KNMusicDetailTooltip;
class KNMusicListViewHeader;
class KNMusicListView : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListView(QWidget *parent = 0);
    virtual void resetHeader();
    void moveToFirst(const int &logicalHeaderIndex);

signals:
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    bool event(QEvent *event);

private slots:
    void changeBackground(int frameData);
    void onSectionVisibleChanged(const int &index,
                                 const bool &visible);
    void onItemActived(const QModelIndex &index);

private:
    KNMusicListViewHeader *m_headerWidget;
    QTimeLine *m_mouseIn, *m_mouseOut;
    QColor m_backgroundColor;
    QPalette m_palette;
    QModelIndex m_detailIndex;
    KNMusicDetailTooltip *m_musicDetailTooltip;
};

#endif // KNMUSICLISTVIEW_H
