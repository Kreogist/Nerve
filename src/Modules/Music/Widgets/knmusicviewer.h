#ifndef KNMUSICVIEWER_H
#define KNMUSICVIEWER_H

#include <QList>
#include <QUrl>
#include <QModelIndex>

#include "../../Base/knstdlibviewer.h"

class QDragEnterEvent;
class QDropEvent;
class KNMusicListView;
class KNMusicArtistView;
class KNMusicViewerMenu;
class KNMusicViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNMusicViewer(QWidget *parent = 0);
    void setDefaultHeader();
    void setModel(QAbstractItemModel *model);

signals:
    void requireAnalysisUrls(QList<QUrl> urls);

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void showContextMenu(const QPoint &position,
                          const QModelIndex &index);

private:
    enum MusicCategories
    {
        Songs,
        Artists,
        Albums,
        Genres,
        Playlists,
        MusicCategoriesCount
    };
    QString m_categoryCaption[MusicCategoriesCount];
    KNMusicListView *m_libraryView;
    KNMusicArtistView *m_artistView;
    KNMusicViewerMenu *m_libraryViewMenu;
};

#endif // KNMUSICVIEWER_H
