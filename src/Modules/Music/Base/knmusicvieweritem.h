#ifndef KNMUSICVIEWERITEM_H
#define KNMUSICVIEWERITEM_H

#include <QWidget>
#include <QObject>

class QSortFilterProxyModel;
class KNMusicLibraryModelBase;
class KNMusicViewerItem : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicViewerItem(QObject *parent = 0);
    virtual void setSourceModel(KNMusicLibraryModelBase *model)=0;
    virtual QWidget *widget()=0;

signals:
    void requirePlayMusic(const QString &filePath);
    void requireSetProxy(QSortFilterProxyModel *model);
    void requireShowContextMenu(const QPoint &position,
                                int currentMode);

public slots:

};

#endif // KNMUSICVIEWERITEM_H
