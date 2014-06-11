#ifndef KNMUSICCATEGORYVIEW_H
#define KNMUSICCATEGORYVIEW_H

#include <QTreeView>
#include <QSplitter>
#include <QRadialGradient>

class QBoxLayout;
class QLabel;
class QStandardItemModel;
class QResizeEvent;
class QShowEvent;
class QGraphicsOpacityEffect;
class KNMusicCategoryList;
class KNMusicCategoryModel;
class KNMusicCategoryDetailModel;
class KNMusicListViewBase;
class KNMusicCategorySortFilterModel;
class KNMusicCategoryDetailsDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicCategoryDetailsDisplay(QWidget *parent = 0);
    void setArtistName(const QString &artistName);
    void setSongNumber(const int &index);
    void setDetailModel(KNMusicCategoryDetailModel *model);
    void setCurrentIndex(const QModelIndex &index);
    void setSongListView(KNMusicListViewBase *listview);
    void setBackground(const QIcon &background);
    void resetHeader();

signals:

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QLabel *m_artistName, *m_artistInfo, *m_largeIcon;
    QString m_song, m_songs;
    KNMusicListViewBase *m_songViewer;
    QBoxLayout *m_layout;
    QGraphicsOpacityEffect *m_opacityEffect;
    QRadialGradient m_alphaGradient;
};

class KNMusicCategoryView : public QSplitter
{
    Q_OBJECT
public:
    explicit KNMusicCategoryView(QWidget *parent = 0);
    void resetHeader();
    void setModel(KNMusicCategorySortFilterModel *model);
    void setDetailModel(KNMusicCategoryDetailModel *model);
    void selectCategoryItem(const QString &value);
    void selectItem(const QModelIndex &index);
    void setSongListView(KNMusicListViewBase *listview);

signals:

public slots:
    void onActionSongCountChange(const int &value);

protected:
    void showEvent(QShowEvent *event);

private slots:
    void onActionItemActivate(const QModelIndex &current,
                              const QModelIndex &previous);

private:
    KNMusicCategoryList *m_artistList;
    KNMusicCategoryDetailsDisplay *m_artistDetails;
    KNMusicCategoryModel *m_artistModel;
    KNMusicCategoryDetailModel *m_artistDetailModel;
    KNMusicCategorySortFilterModel *m_proxyModel;
};

#endif // KNMUSICCATEGORYVIEW_H
