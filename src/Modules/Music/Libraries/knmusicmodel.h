#ifndef KNMUSICMODEL_H
#define KNMUSICMODEL_H

#include "../knmusicglobal.h"

#include "../../Base/knmodel.h"

class KNMusicInfoCollectorManager;
class KNMusicModel : public KNModel
{
    Q_OBJECT
public:
    explicit KNMusicModel(QObject *parent = 0);
    QString filePathFromIndex(const QModelIndex &index);
    QString itemText(const int &row, const int &column) const;
    QVariant itemRoleData(int row, int column, int role) const;
    QPixmap itemArtwork(const int &row) const;
    void addRawFileItem(QString filePath);
    void addRawFileItems(QStringList fileList);
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

public slots:
    void retranslate();
    void retranslateAndSet();

protected slots:
    void onActionUpdateRowInfo();

private:
    void setMusicDetailsInfo(const int &currentRow,
                             const KNMusicGlobal::MusicDetailsInfo &currentDetails);
    KNMusicGlobal *m_musicGlobal;
    KNMusicInfoCollectorManager *m_infoCollectorManager;
    int m_rawFileCount=0;

    //Debug
    int m_startTime;
};

#endif // KNMUSICMODEL_H
