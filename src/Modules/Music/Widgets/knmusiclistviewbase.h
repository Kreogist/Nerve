#ifndef KNMUSICLISTVIEWBASE_H
#define KNMUSICLISTVIEWBASE_H

#include <QTreeView>

class KNMusicListViewBase : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListViewBase(QWidget *parent = 0);
    void setHeaderAlignment(const int &logicalHeaderIndex,
                            Qt::Alignment alignment);
    virtual void resetHeader();
    void moveToFirst(const int &logicalHeaderIndex);

signals:
    void requireShowContextMenu(const QPoint &position);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    virtual void retranslate();
    virtual void retranslateAndSet();

protected slots:
    virtual void onActionSort(int logicalIndex, Qt::SortOrder order);
};

#endif // KNMUSICLISTVIEWBASE_H
