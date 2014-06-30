#ifndef KNMUSICVIEWERITEMBASE_H
#define KNMUSICVIEWERITEMBASE_H

#include <QWidget>
#include <QObject>

class QSortFilterProxyModel;
class KNMusicBackend;
class KNMusicLibraryModelBase;
class KNMusicViewerItemBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicViewerItemBase(QObject *parent = 0);
    virtual void applyPlugin()=0;

signals:
    void requirePlayMusic(const QModelIndex &sourceIndex);
    void requireSetProxy(QSortFilterProxyModel *model);
    void requireShowContextMenu(const QPoint &position,
                                int currentMode);
    void requireAddCategory(const QPixmap &icon,
                            const QString &title,
                            QWidget *widget);

public slots:
    virtual void setMusicSourceModel(KNMusicLibraryModelBase *model)=0;
    virtual void onActionResort();
    virtual void onActionSearch(const QString &text);
    virtual void onActionShowIndex(const QModelIndex &index);
    virtual void setBackend(KNMusicBackend *backend)=0;
    virtual void onActionRemoveItem(const QModelIndex &index);
    virtual void onActionRemoveOriginalItem(const QModelIndex &index);

protected slots:
    virtual void onActionShowContextMenu(const QPoint &position);

};

#endif // KNMUSICVIEWERITEMBASE_H
