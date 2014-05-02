#ifndef KNMUSICARTISTVIEW_H
#define KNMUSICARTISTVIEW_H

#include <QTreeView>
#include <QSplitter>
#include <QThread>

class QBoxLayout;
class QLabel;
class QStandardItemModel;
class KNMusicCategoryList;
class KNMusicCategoryModel;
class KNMusicCategoryDetailModel;
class KNMusicListView;

class KNMusicCategoryDetailsDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicCategoryDetailsDisplay(QWidget *parent = 0);
    void setArtistName(const QString &artistName);
    void setSongNumber(const int &index);
    void setDetailModel(KNMusicCategoryDetailModel *model);
    void setCurrentIndex(const QModelIndex &index);
    void setSongListView(KNMusicListView *listview);
    void resetHeader();

signals:

private:
    QLabel *m_artistName, *m_artistInfo;
    QString m_song, m_songs;
    KNMusicListView *m_songViewer;
    QBoxLayout *m_layout;
};

class KNMusicArtistView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicArtistView(QWidget *parent = 0);
    void resetHeader();
    void setModel(KNMusicCategoryModel *model);
    void setDetailModel(KNMusicCategoryDetailModel *model);
    void selectCategoryItem(const QString &value);
    void selectItem(const QModelIndex &index);
    void setSongListView(KNMusicListView *listview);

signals:

public slots:
    void resort();
    void onActionSongCountChange(const int &value);

private slots:
    void onActionItemActivate(const QModelIndex &current,
                              const QModelIndex &previous);

private:
    KNMusicCategoryList *m_artistList;
    KNMusicCategoryDetailsDisplay *m_artistDetails;
    KNMusicCategoryModel *m_artistModel;
    KNMusicCategoryDetailModel *m_artistDetailModel;
};

#endif // KNMUSICARTISTVIEW_H
