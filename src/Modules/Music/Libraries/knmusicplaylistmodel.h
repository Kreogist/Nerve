#ifndef KNMUSICPLAYLISTMODEL_H
#define KNMUSICPLAYLISTMODEL_H

#include <QThread>
//#include <QStringList>

#include "../knmusicglobal.h"

#include "../../Base/knmodel.h"

class KNMusicInfoCollectorManager;
class KNMusicPlaylistModel : public KNModel
{
    Q_OBJECT
public:
    explicit KNMusicPlaylistModel(QObject *parent = 0);
//    void setPlaylist(const QStringList &fileList);

    void resetHeader();
    QString filePathFromIndex(const QModelIndex &index);
    QString filePathFromIndex(const int &index);
    QModelIndex indexFromFilePath(const QString &filePath);
    QString itemText(const int &row, const int &column) const;
    QVariant itemRoleData(int row, int column, int role) const;
    QList<QStandardItem *> songRow(const int &row) const;
    void addRawFileItem(QString filePath);
    void addRawFileItems(QStringList fileList);
    void updateIndexInfo(const QModelIndex &index,
                         const QString &filePath);
    void setInfoCollectorManager(KNLibInfoCollectorManager *infoCollectorManager);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected slots:
    void onActionUpdateRowInfo();

private:
    enum MusicProperty
    {
        FilePathRole=Qt::UserRole,
        FileNameRole,
        ArtworkKeyRole,
    };

    void setMusicDetailsInfo(const int &currentRow,
                             const KNMusicGlobal::MusicDetailsInfo &currentDetails);
    KNMusicGlobal *m_musicGlobal;
    KNMusicInfoCollectorManager *m_infoCollectorManager;
    bool m_imageRecoverComplete=false, m_dataRecoverComplete=false,
         m_doImageCache=true;
};

#endif // KNMUSICPLAYLISTMODEL_H
