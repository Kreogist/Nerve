#ifndef KNMUSICPLAYLISTLISTMODEL_H
#define KNMUSICPLAYLISTLISTMODEL_H

#include <QList>
#include <QUrl>

#include <QStandardItemModel>

class KNMusicPlaylistListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListModel(QObject *parent = 0);
    Qt::DropActions supportedDropActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

signals:
    void requireAddToPlaylist(int rows, QList<QUrl> fileList);
    void requireCreatePlaylist(QList<QUrl> fileList);

public slots:

protected:

};

#endif // KNMUSICPLAYLISTLISTMODEL_H
