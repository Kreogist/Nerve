#ifndef KNMUSICPLAYLISTLISTEDITORBASE_H
#define KNMUSICPLAYLISTLISTEDITORBASE_H

#include <QWidget>

class KNMusicPlaylistListEditorBase : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistListEditorBase(QWidget *parent = 0);

signals:
    void requireAddPlaylist();

public slots:

};

#endif // KNMUSICPLAYLISTLISTEDITORBASE_H
