#ifndef KNMUSICPLAYLISTMODEL_H
#define KNMUSICPLAYLISTMODEL_H

#include <QStandardItemModel>
#include <QStringList>

class KNMusicModel;
class KNMusicPlaylistModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistModel(QObject *parent = 0);
    void setPlaylist(const QStringList &fileList);
    void setMusicModel(KNMusicModel *model);

signals:

public slots:

private:
    KNMusicModel *m_model;
};

#endif // KNMUSICPLAYLISTMODEL_H
