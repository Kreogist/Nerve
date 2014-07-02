#ifndef KNMUSICLISTVIEWITEM_H
#define KNMUSICLISTVIEWITEM_H

#include "../Base/knmusicvieweritembase.h"

class KNMusicListView;
class KNMusicLibraryModelBase;
class KNMusicViewContainer;
class KNMusicSortModel;
class KNMusicListViewItem : public KNMusicViewerItemBase
{
    Q_OBJECT
public:
    explicit KNMusicListViewItem(QObject *parent = 0);
    ~KNMusicListViewItem();
    void applyPlugin();

signals:

public slots:
    void onActionResort();
    void onActionSearch(const QString &text);
    void onActionShowIndex(const QModelIndex &index);
    void onActionRemoveOriginalItem(const QModelIndex &index);
    void retranslate();
    void setMusicSourceModel(KNMusicLibraryModelBase *model);
    void setBackend(KNMusicBackend *backend);

protected slots:
    void onActionShowContextMenu(const QPoint &position);

private:
    KNMusicSortModel *m_listViewModel;
    KNMusicListView *m_libraryView;
    KNMusicViewContainer *m_container;
    QString m_captionTitle;
};

#endif // KNMUSICLISTVIEWITEM_H
