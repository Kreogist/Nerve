#ifndef KNMUSICVIEWER_H
#define KNMUSICVIEWER_H

#include "../../Base/knstdlibviewer.h"

class KNMusicListView;
class KNMusicViewer : public KNStdLibViewer
{
    Q_OBJECT
public:
    explicit KNMusicViewer(QWidget *parent = 0);

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
    KNMusicListView *test[MusicCategoriesCount];
    QString m_categoryCaption[MusicCategoriesCount];
};

#endif // KNMUSICVIEWER_H
