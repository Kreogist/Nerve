#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include <QThread>

#include "../knmusicglobal.h"

#include "knmusicmodelbase.h"

class KNLibHashPixmapList;
class KNMusicInfoCollectorManager;
class KNMusicModel : public KNMusicModelBase
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);
    ~KNMusicModel();
    QImage artwork(const int &row) const;
    QImage artworkFromKey(const QString &key) const;
    QString itemArtworkKey(const int &row) const;
    void addRawFileItem(QString filePath);
    void addRawFileItems(QStringList fileList);
    void setAlbumArtPath(const QString &path);
    void recoverFile(QStringList textList,
                     KNMusicGlobal::MusicDetailsInfo currentDetails);
    void updateIndexInfo(const QModelIndex &index,
                         const QString &filePath);
    void prepareRemove(const QModelIndex &index);
    void setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager);

signals:
    void musicAppend(QModelIndex index);
    void musicAboutToRemove(QModelIndex index);
    void musicUpdate(QModelIndex index);
    void musicRecover(QModelIndex index);
    void musicAlbumArtUpdate(int index);
    void requireLoadImage();
    void requireUpdateImage();

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

#endif // KNMUSICMODEL_H
