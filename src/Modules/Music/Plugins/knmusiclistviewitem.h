#ifndef KNMUSICLISTVIEWITEM_H
#define KNMUSICLISTVIEWITEM_H

#include "../Base/knmusicvieweritem.h"

class KNMusicListView;
class KNMusicLibraryModelBase;
class KNMusicSortModel;
class KNMusicListviewItem : public KNMusicViewerItem
{
    Q_OBJECT
public:
    explicit KNMusicListviewItem(QObject *parent = 0);
    ~KNMusicListviewItem();
    void setSourceModel(KNMusicLibraryModelBase *model);
    QWidget *widget();

signals:

public slots:

private slots:
    void onActionShowContextMenu(const QPoint &position);

private:
    KNMusicSortModel *m_listViewModel;
    KNMusicListView *m_libraryView;
};

#endif // KNMUSICLISTVIEWITEM_H
