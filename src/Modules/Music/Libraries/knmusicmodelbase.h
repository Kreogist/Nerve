#ifndef KNMUSICMODELBASE_H
#define KNMUSICMODELBASE_H

#include "../knmusicglobal.h"

#include "../../Base/knmodel.h"

class KNGlobal;
class KNMusicGlobal;
class KNMusicModelBase : public KNModel
{
    Q_OBJECT
public:
    explicit KNMusicModelBase(QObject *parent = 0);
    void resetHeader();
    QString filePathFromIndex(const QModelIndex &index);
    QString filePathFromIndex(const int &row);
    QModelIndex indexFromFilePath(const QString &filePath);
    QString itemText(const int &row, const int &column) const;
    QVariant itemRoleData(int row, int column, int role) const;
    QList<QStandardItem *> songRow(const int &row) const;
    QModelIndex appendMusicItem(const QStringList &textList,
                                const KNMusicGlobal::MusicDetailsInfo &currentDetails);

signals:

public slots:

protected:
    void setMusicDetailsInfo(const int &currentRow,
                             const KNMusicGlobal::MusicDetailsInfo &currentDetails);

private:
    KNGlobal *m_global;
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICMODELBASE_H
