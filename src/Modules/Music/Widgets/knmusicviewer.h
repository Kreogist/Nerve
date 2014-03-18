#ifndef KNMUSICVIEWER_H
#define KNMUSICVIEWER_H

#include "../../Base/knstdlibviewer.h"

class KNMusicListView;
class KNMusicModel;
class KNMusicViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNMusicViewer(QWidget *parent = 0);
    void setDefaultHeader();

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

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
    KNMusicModel *m_model;
};

#endif // KNMUSICVIEWER_H
