#ifndef KNMUSICALBUMVIEW_H
#define KNMUSICALBUMVIEW_H

#include <QAbstractItemView>

class QBoxLayout;
class QLabel;
class QMouseEvent;
class QPaintEvent;
class QPropertyAnimation;
class QParallelAnimationGroup;
class QTimeLine;
class KNMusicAlbumSongListView;
class KNMusicAlbumDetailModel;
class KNMusicAlbumModel;
class KNMusicCategorySortFilterModel;
class KNMusicAlbumSongDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumSongDetail(QWidget *parent = 0);
    ~KNMusicAlbumSongDetail();
    void setAlbumName(const QString &name);
    void setArtistName(const QString &name);
    void setDetailModel(KNMusicAlbumDetailModel *model);
    void selectItem(const QModelIndex &index);
    void resetHeader();
    void resetSongState();

signals:
    void requireOpenUrl(const QModelIndex &index);
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);

public slots:
    void hideDetailInfo();
    void showDetailInfo();

private:
    QLabel *m_albumName;
    QLabel *m_artistName;
    KNMusicAlbumSongListView *m_albumSongs;
    QBoxLayout *m_mainLayout;
    QBoxLayout *m_detailLayout;
};

class KNMusicAlbumInfoDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumInfoDetail(QWidget *parent = 0);
    enum AlbumInfoData
    {
        SongCount,
        Year,
        AlbumInfoDataCount
    };
    int minimalExpandedHeight() const;
    void setCaption(const int &index, const QString &value);

signals:
    void changeInfoVisible(const bool &visible);

public slots:
    void retranslate();
    void retranslateAndSet();
    void hideDetailInfo();
    void showDetailInfo();
    void onActionSongCountChange(const int &value);

private:
    void updateSongCount();
    int m_minimalExpandedHeight;
    QBoxLayout *m_albumDataLayout;
    QLabel *m_albumInfo[AlbumInfoDataCount];
    QString m_songCountText, m_songsCountText;
    int m_songCount;
};

class KNMusicAlbumDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumDetail(QWidget *parent = 0);
    ~KNMusicAlbumDetail();
    void setAlbumArt(const QPixmap &pixmap,
                     const QSize &size);
    void setAlbumName(const QString &name);
    void setArtistName(const QString &name);
    void setYear(const QString &value);
    void setDetailModel(KNMusicAlbumDetailModel *model);
    void selectItem(const QModelIndex &index);
    void resetHeader();
    void resetSongState();

signals:
    void requireOpenUrl(const QModelIndex &index);
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);
    void requireFlyBack();
    void requireFlyOut();

public slots:
    void hideDetailWidget();
    void showDetailWidget();
    void expandDetail();
    void foldDetail();
    void flyAway();

private slots:
    void hideDetailContent();
    void showDetailContent();

private:
    QLabel *m_albumArt;
    KNMusicAlbumInfoDetail *m_infoPanel;
    KNMusicAlbumSongDetail *m_songPanel;
    QBoxLayout *m_infoListLayout, *m_artInfoLayout;
    QPropertyAnimation *m_heightExpand, *m_widthExpand, *m_heightFold, *m_widthFold,
                       *m_flyOut;
};

class KNMusicAlbumView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit KNMusicAlbumView(QWidget *parent = 0);
    QModelIndex indexAt(const QPoint &point) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QRect visualRect(const QModelIndex &index) const;
    void setModel(QAbstractItemModel *model);
    void setCategoryModel(KNMusicCategorySortFilterModel *model);
    void setDetailModel(KNMusicAlbumDetailModel *model);
    void selectCategoryItem(const QString &value);
    void selectItem(const QModelIndex &index);
    int gridMinimumWidth() const;
    void setGridMinimumWidth(int gridMinimumWidth);
    void resetHeader();
    void setFilterFixedString(const QString &text);

signals:
    void requireOpenUrl(const QModelIndex &index);
    void requireShowContextMenu(const QPoint &position,
                                const QModelIndex &index);

public slots:
    void selectAlbum(const QModelIndex &index);

protected slots:
    void updateGeometries();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                                        Qt::KeyboardModifiers modifiers);
    void setSelection(const QRect &rect,
                      QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void expandAlbumDetails(const QModelIndex &index);
    void onActionHideAlbumDetail();
    void onActionHideAlbumDetailFinished();
    void onActionAlbumRemoved(const QModelIndex &index);
    void onActionFlyAwayAlbumDetail();
    void onActionFlyAwayAlbumDetailFinished();
    void showFirstItem();
    void hideFirstItem();

private:
    void flyAwayAlbumDetail();
    void foldAlbumDetail();
    QRect itemRect(const QModelIndex &index) const;
    void updateParameters();
    void paintAlbum(QPainter *painter,
                    const QRect &rect,
                    const QModelIndex &index);
    int m_gridMinimumWidth=124;
    int m_gridWidth=124;
    int m_gridHeight=144;
    int m_spacing=10;
    int m_maxColumnCount=1;
    int m_firstVisibleIndex=0;
    int m_lineCount=0;
    int m_iconSizeParam=124;
    int m_spacingHeight=154, m_spacingWidth=134;
    int m_minGrey=0x30;
    bool m_hidingAlbum=false, m_flyingAlbum=false, m_noAlbumHide=false;
    QPalette m_palette;
    QColor m_backgroundColor;
    QTimeLine *m_scrollTimeLine;
    QPropertyAnimation *m_albumShow, *m_albumHide,
                       *m_albumThrow;
    QParallelAnimationGroup *m_flyawayGroup;
    KNMusicAlbumDetail *m_albumDetail;
    QModelIndex m_detailIndex, m_pressedIndex;
    KNMusicAlbumDetailModel *m_detailModel;
    KNMusicAlbumModel *m_model;
    KNMusicCategorySortFilterModel *m_proxyModel;
};

#endif // KNMUSICALBUMVIEW_H
