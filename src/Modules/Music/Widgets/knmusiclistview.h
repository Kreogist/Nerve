#ifndef KNMUSICLISTVIEW_H
#define KNMUSICLISTVIEW_H

#include <QTreeView>

class KNMusicListViewHeader;
class KNMusicListView : public QTreeView
{
    Q_OBJECT
public:
    explicit KNMusicListView(QWidget *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    KNMusicListViewHeader *m_headerWidget;
};

#endif // KNMUSICLISTVIEW_H
