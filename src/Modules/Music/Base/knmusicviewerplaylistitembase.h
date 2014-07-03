#ifndef KNMUSICVIEWERPLAYLISTITEMBASE_H
#define KNMUSICVIEWERPLAYLISTITEMBASE_H

#include <QWidget>
#include <QObject>

class KNMusicPlaylistManagerBase;
class KNMusicViewerPlaylistItemBase : public QObject
{
    Q_OBJECT
public:
    explicit KNMusicViewerPlaylistItemBase(QObject *parent = 0);
    virtual void applyPlugin()=0;
    virtual QWidget *playlistListView()=0;

signals:
    void requirePlayMusic(const QString &filePath);
    void requireAddCategory(const QPixmap &icon,
                            const QString &title,
                            QWidget *widget);

public slots:
    virtual void setPlaylistManager(KNMusicPlaylistManagerBase *manager)=0;

};

#endif // KNMUSICVIEWERPLAYLISTITEMBASE_H
