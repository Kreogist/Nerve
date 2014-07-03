#ifndef KNMUSICCATEGORYVIEW_H
#define KNMUSICCATEGORYVIEW_H

#include <QUrl>
#include <QList>
#include <QTreeView>
#include <QSplitter>
#include <QRadialGradient>

class QBoxLayout;
class QLabel;
class QStandardItemModel;
class QResizeEvent;
class QDragEnterEvent;
class QDropEvent;
class QShowEvent;
class QGraphicsOpacityEffect;
class KNMusicLibraryModelBase;
class KNMusicCategoryList;
class KNMusicCategoryModel;
class KNMusicCategoryDetailModel;
class KNMusicLibraryListview;
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
    void setSongListView(KNMusicLibraryListview *listview);
    void setMusicSourceModel(KNMusicLibraryModelBase *model);
    void setBackground(const QIcon &background);
    void resetHeader();

signals:

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QLabel *m_artistName, *m_artistInfo, *m_largeIcon;
    QString m_song, m_songs;
    KNMusicLibraryListview *m_songViewer;
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
    void setMusicSourceModel(KNMusicLibraryModelBase *model);
    void setModel(KNMusicCategorySortFilterModel *model);
    void setDetailModel(KNMusicCategoryDetailModel *model);
    void selectMusicItem(const QModelIndex &index);
    void setSongListView(KNMusicLibraryListview *listview);

signals:
    void requireAnalysisUrls(QList<QUrl> urls);
    void dragEntered();
    void dropped();

public slots:
    void onActionSongCountChange(const int &value);

protected:
    void showEvent(QShowEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void onActionItemActivate(const QModelIndex &current,
                              const QModelIndex &previous);

private:
    void selectCategoryItem(const QString &value);
    void selectItem(const QModelIndex &index);
    KNMusicCategoryList *m_artistList;
    KNMusicCategoryDetailsDisplay *m_artistDetails;
    KNMusicCategoryModel *m_artistModel;
    KNMusicCategoryDetailModel *m_artistDetailModel;
    KNMusicCategorySortFilterModel *m_proxyModel;
    KNMusicLibraryModelBase *m_musicModel;
};

#endif // KNMUSICCATEGORYVIEW_H
