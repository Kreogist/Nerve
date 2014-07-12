#ifndef KNMUSICPLAYLISTINDEX_H
#define KNMUSICPLAYLISTINDEX_H

#include <QStyledItemDelegate>

class KNMusicPlaylistIndex : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistIndex(QObject *parent = 0);
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
signals:

public slots:

};

#endif // KNMUSICPLAYLISTINDEX_H
