#ifndef KNMUSICLISTVIEW_H
#define KNMUSICLISTVIEW_H

#include <QTreeView>

class QTimeLine;
class KNMusicListViewHeader;
class KNMusicListView : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListView(QWidget *parent = 0);
    void resetHeader();

signals:
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private slots:

private:
    KNMusicListViewHeader *m_headerWidget;
    QTimeLine *m_mouseIn;
};

#endif // KNMUSICLISTVIEW_H
