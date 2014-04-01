#ifndef KNMUSICALBUMVIEW_H
#define KNMUSICALBUMVIEW_H

#include <QAbstractItemView>

class QPaintEvent;
class KNMusicAlbumView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit KNMusicAlbumView(QWidget *parent = 0);
    QModelIndex indexAt(const QPoint &point) const;
    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    QRect visualRect(const QModelIndex &index) const;

    int gridMinimumWidth() const;
    void setGridMinimumWidth(int gridMinimumWidth);

signals:

public slots:

protected slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

protected:
    void paintEvent(QPaintEvent *event);
    int horizontalOffset() const;
    int verticalOffset() const;
    bool isIndexHidden(const QModelIndex &index) const;
    QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction,
                                        Qt::KeyboardModifiers /*modifiers*/);
    void setSelection(const QRect&, QItemSelectionModel::SelectionFlags command);
    QRegion visualRegionForSelection(const QItemSelection &selection) const;

private:
    int m_gridMinimumWidth=195;
    int m_gridWidth=195;
    int m_gridHeight=190;
    int m_spacing=20;
};

#endif // KNMUSICALBUMVIEW_H
