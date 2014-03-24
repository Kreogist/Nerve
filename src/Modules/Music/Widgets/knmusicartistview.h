#ifndef KNMUSICARTISTVIEW_H
#define KNMUSICARTISTVIEW_H

#include <QSplitter>

class QListView;
class QLabel;
class QAbstractItemModel;
class KNMusicArtistModel;
class KNMusicArtistDetailsDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicArtistDetailsDisplay(QWidget *parent = 0);
    void setModel(KNMusicArtistModel *model);
    void setArtistName(const QString &artistName);
    void setSongNumber(const int &index);

private:
    QLabel *m_artistName, *m_artistInfo;
    KNMusicArtistModel *m_model;
    QString m_song, m_songs;
};

class KNMusicArtistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicArtistView(QWidget *parent = 0);
    void setModel(KNMusicArtistModel *model);

signals:

public slots:
    void resort();

private slots:
    void onActionItemActivate(const QModelIndex &index);

private:
    QListView *m_artistList;
    KNMusicArtistDetailsDisplay *m_artistDetails;
    KNMusicArtistModel *m_artistModel;
};

#endif // KNMUSICARTISTVIEW_H
