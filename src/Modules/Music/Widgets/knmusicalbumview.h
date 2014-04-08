#ifndef KNMUSICALBUMVIEW_H
#define KNMUSICALBUMVIEW_H

#include <QAbstractItemView>

class QTimeLine;
class QMouseEvent;
class QPaintEvent;
class QLabel;
class QPropertyAnimation;
class QBoxLayout;

class KNMusicAlbumSongDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumSongDetail(QWidget *parent = 0);

private:

};

class KNMusicAlbumInfoDetail : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicAlbumInfoDetail(QWidget *parent = 0);
    void setAlbumName(const QString &name);
    enum AlbumInfoData
    {
        AlbumName,
        AlbumInfoDataCount
    };
    int minimalExpandedHeight() const;

signals:
    void changeInfoVisible(const bool &visible);

public slots:
    void hideDetailInfo();
    void showDetailInfo();

private:
    int m_minimalExpandedHeight;
    QBoxLayout *m_albumDataLayout;
    QLabel *m_albumInfo[AlbumInfoDataCount];
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
    QModelIndex currentIndex() const;
    void setCurrentIndex(const QModelIndex &currentIndex);

signals:
    void requireFlyBack();

public slots:
    void hideDetailWidget();
    void showDetailWidget();
    void expandDetail();
    void foldDetail();

private slots:
    void hideDetailContent();
    void showDetailContent();

private:
    QLabel *m_albumArt;
    KNMusicAlbumInfoDetail *m_infoPanel;
    QWidget *m_songPanel;
    QBoxLayout *m_infoListLayout, *m_artInfoLayout;
    QPropertyAnimation *m_heightExpand, *m_widthExpand, *m_heightFold, *m_widthFold;
    QModelIndex m_currentIndex;
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

    int gridMinimumWidth() const;
    void setGridMinimumWidth(int gridMinimumWidth);

signals:

public slots:

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void updateGeometries();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                                        Qt::KeyboardModifiers /*modifiers*/);
    void setSelection(const QRect &rect,
                      QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void onActionAlbumClicked(const QModelIndex &index);
    void onActionHideAlbumDetail();

private:
    QRect itemRect(const QModelIndex &index) const;
    void paintAlbum(QPainter *painter,
                    const QRect &rect,
                    const QModelIndex &index);
    int m_gridMinimumWidth=124;
    int m_gridWidth=124;
    int m_gridHeight=180;
    int m_spacing=20;
    int m_maxColumnCount=0;
    int m_firstVisibleIndex=0;
    int m_lineCount=0;
    int m_iconSizeParam;
    QPalette m_palette;
    QColor m_backgroundColor;
    int m_minGrey=0x30;
    QModelIndex m_pressedIndex;
    QTimeLine *m_scrollTimeLine;
    QPropertyAnimation *m_albumShow,
                       *m_albumHide;
    KNMusicAlbumDetail *m_albumDetail;
};

#endif // KNMUSICALBUMVIEW_H
