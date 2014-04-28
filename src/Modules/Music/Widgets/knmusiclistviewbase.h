#ifndef KNMUSICLISTVIEWBASE_H
#define KNMUSICLISTVIEWBASE_H

#include <QTreeView>

class KNMusicDetailTooltip;
class KNMusicListViewHeader;
class KNMusicListViewBase : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListViewBase(QWidget *parent = 0);
    virtual void resetHeader();
    void moveToFirst(const int &logicalHeaderIndex);

signals:
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();

private slots:
    void onSectionVisibleChanged(const int &index,
                                 const bool &visible);
    void onItemActived(const QModelIndex &index);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);

private:
    KNMusicListViewHeader *m_headerWidget;
    QModelIndex m_detailIndex;
    KNMusicDetailTooltip *m_musicDetailTooltip;
};

#endif // KNMUSICLISTVIEWBASE_H
