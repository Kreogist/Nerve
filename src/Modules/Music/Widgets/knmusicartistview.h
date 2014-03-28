#ifndef KNMUSICARTISTVIEW_H
#define KNMUSICARTISTVIEW_H

#include <QTreeView>
#include <QListView>
#include <QSplitter>
#include <QThread>
#include <QScopedPointer>

#include "knmusiclistview.h"

class QLabel;
class QStandardItemModel;
class KNMusicArtistList;
class KNMusicCategoryModel;
class KNMusicCategoryDetailModel;
class KNMusicArtistSongs : public KNMusicListView
{
    Q_OBJECT
public:
    explicit KNMusicArtistSongs(QWidget *parent = 0);
    void resetHeader();

private:
};

class KNMusicArtistDetailsDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicArtistDetailsDisplay(QWidget *parent = 0);
    void setArtistName(const QString &artistName);
    void setSongNumber(const int &index);
    void setDetailModel(KNMusicCategoryDetailModel *model);
    void resetHeader();

signals:
    void requireOpenUrl(const QModelIndex &index);

private:
    QLabel *m_artistName, *m_artistInfo;
    QString m_song, m_songs;
    KNMusicArtistSongs *m_songViewer;
};

class KNMusicArtistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicArtistView(QWidget *parent = 0);
    void resetHeader();
    void setModel(KNMusicCategoryModel *model);
    void setDetailModel(KNMusicCategoryDetailModel *model);

signals:
    void requireDisplayDetails(const QModelIndex &current);
    void requireOpenUrl(const QModelIndex &index);

public slots:
    void resort();
    void onActionDetailCountChange(const int &value);

private slots:
    void onActionItemActivate(const QModelIndex & current,
                              const QModelIndex & previous);

private:
    KNMusicArtistList *m_artistList;
    KNMusicArtistDetailsDisplay *m_artistDetails;
    KNMusicCategoryModel *m_artistModel;
};

#endif // KNMUSICARTISTVIEW_H
