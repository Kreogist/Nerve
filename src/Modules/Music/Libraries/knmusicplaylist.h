#ifndef KNMUSICPLAYLIST_H
#define KNMUSICPLAYLIST_H

#include <QObject>

class KNMusicModel;
class KNMusicPlayList : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicPlayList(QObject *parent = 0);
    void setModel(KNMusicModel *model);

signals:

public slots:

private:
    KNMusicModel *m_model;
};

#endif // KNMUSICPLAYLIST_H
