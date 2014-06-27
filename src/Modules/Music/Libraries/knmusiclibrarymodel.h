#ifndef KNMUSICLIBRARYMODEL_H
#define KNMUSICLIBRARYMODEL_H

#include <QThread>

#include "knmusiclibrarymodelbase.h"

class KNLibHashPixmapList;
class KNMusicInfoCollectorManager;
class KNMusicLibraryModel : public KNMusicLibraryModelBase
{
    Q_OBJECT
public:
    explicit KNMusicLibraryModel(QObject *parent = 0);
    ~KNMusicLibraryModel();
    QImage artwork(const int &row) const;
    QImage artworkFromKey(const QString &key) const;
    QString artworkKey(const int &row) const;
    void addRawFileItem(QString filePath);
    void addRawFileItems(QStringList fileList);
    void setAlbumArtPath(const QString &path);
    void recoverFile(QStringList textList,
                     KNMusicGlobal::MusicDetailsInfo currentDetails);
    void updateIndexInfo(const QModelIndex &index,
                         const QString &filePath);
    void prepareRemove(const QModelIndex &removedIndex);
    void setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();
    void onActionRecoverComplete();

protected slots:
    void onActionUpdateRowInfo();
    void onActionImageLoadComplete();

private slots:
    void onActionUpdatePixmap();

private:
    KNMusicGlobal *m_musicGlobal;
    KNMusicInfoCollectorManager *m_infoCollectorManager;
    KNLibHashPixmapList *m_pixmapList;
    QThread m_pixmapListThread;
    int m_rawFileCount=0;
    bool m_imageRecoverComplete=false, m_dataRecoverComplete=false;

    //Debug
    int m_startTime;
};

#endif // KNMUSICLIBRARYMODEL_H
