#ifndef KNMUSICALBUMVIEW_H
#define KNMUSICALBUMVIEW_H

#include <QListView>
#include <QStyledItemDelegate>

class KNMusicAlbumDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit KNMusicAlbumDelegate(QObject *parent = 0);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
};

class KNMusicAlbumView : public QListView
{
    Q_OBJECT
public:
    explicit KNMusicAlbumView(QWidget *parent = 0);

signals:

public slots:

protected:
};

#endif // KNMUSICALBUMVIEW_H
