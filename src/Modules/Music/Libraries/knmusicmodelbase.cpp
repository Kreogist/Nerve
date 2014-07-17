#include "../../knglobal.h"

#include <QDebug>

#include "knmusicmodelbase.h"

KNMusicModelBase::KNMusicModelBase(QObject *parent) :
    KNModel(parent)
{
    m_musicGlobal=KNMusicGlobal::instance();
    m_global=KNGlobal::instance();
}

void KNMusicModelBase::resetHeader()
{
    //Set the header text.
    QStringList header;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        header<<(m_musicGlobal->getHeader(i));
    }
    setHorizontalHeaderLabels(header);
    //Set header size hint.
    setHeaderData(0, Qt::Horizontal, QSize(10,23), Qt::SizeHintRole);
    //Set header alignment
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        setHeaderData(i, Qt::Horizontal, Qt::AlignVCenter, Qt::TextAlignmentRole);
    }
    //Set special header data, e.g.: Sort flag.
    setHeaderData(KNMusicGlobal::Time, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(KNMusicGlobal::Size, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(KNMusicGlobal::TrackNumber, Qt::Horizontal, QVariant(Qt::AlignVCenter|Qt::AlignRight), Qt::TextAlignmentRole);
    setHeaderData(KNMusicGlobal::Time, Qt::Horizontal, 2, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DiscNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DiscCount, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::TrackNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::TrackCount, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::Size, Qt::Horizontal, 2, Qt::UserRole);
    setHeaderData(KNMusicGlobal::BitRate, Qt::Horizontal, 3, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DateAdded, Qt::Horizontal, 4, Qt::UserRole);
    setHeaderData(KNMusicGlobal::DateModified, Qt::Horizontal, 4, Qt::UserRole);
    setHeaderData(KNMusicGlobal::LastPlayed, Qt::Horizontal, 4, Qt::UserRole);
}

QString KNMusicModelBase::filePathFromIndex(const QModelIndex &index)
{
    return data(this->index(index.row(), KNMusicGlobal::Name), KNMusicGlobal::FilePathRole).toString();
}

QString KNMusicModelBase::filePathFromIndex(const int &row)
{
    return data(index(row, KNMusicGlobal::Name), KNMusicGlobal::FilePathRole).toString();
}

QModelIndex KNMusicModelBase::indexFromFilePath(const QString &filePath)
{
    QModelIndexList fileCheck=match(index(0,0),
                                    KNMusicGlobal::FilePathRole,
                                    filePath);
    return fileCheck.isEmpty()?QModelIndex():fileCheck.first();
}

QString KNMusicModelBase::itemText(const int &row, const int &column) const
{
    return itemRoleData(row, column, Qt::DisplayRole).toString();
}

QVariant KNMusicModelBase::itemRoleData(int row, int column, int role) const
{
    return data(index(row, column), role);
}

QList<QStandardItem *> KNMusicModelBase::songRow(const int &row) const
{
    QList<QStandardItem *> songRowData;
    if(row>-1 && row<rowCount())
    {
        for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
        {
            songRowData.append(item(row, i)->clone());
        }
    }
    return songRowData;
}

QModelIndex KNMusicModelBase::appendMusicItem(const QStringList &textList,
                                              const KNMusicGlobal::MusicDetailsInfo &currentDetails)
{
    QList<QStandardItem *> songItemList;
    QStandardItem *songItem;
    for(int i=0; i<KNMusicGlobal::MusicDataCount; i++)
    {
        songItem=new QStandardItem(textList.at(i));
        songItem->setEditable(false);
        songItemList.append(songItem);
    }
    appendRow(songItemList);
    int currentRow=songItem->index().row();
    songItem=item(currentRow, KNMusicGlobal::DateAdded);
    songItem->setData(currentDetails.dateAdded, Qt::UserRole);
    setMusicDetailsInfo(currentRow, currentDetails);
    songItem=item(currentRow, KNMusicGlobal::Name);
    songItem->setData(currentDetails.coverImageHash, KNMusicGlobal::ArtworkKeyRole);
    songItem->setData(currentDetails.filePath, KNMusicGlobal::FilePathRole);
    return songItem->index();
}

void KNMusicModelBase::setMusicDetailsInfo(const int &currentRow,
                                           const KNMusicGlobal::MusicDetailsInfo &currentDetails)
{
    QStandardItem *songItem;
    songItem=item(currentRow,KNMusicGlobal::TrackNumber);
    songItem->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    songItem=item(currentRow,KNMusicGlobal::Time);
    songItem->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    songItem->setData(currentDetails.duration, Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::BitRate);
    songItem->setData(QVariant(currentDetails.bitRate), Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::SampleRate);
    songItem->setData(currentDetails.samplingRate, Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::DateModified);
    songItem->setData(currentDetails.dateModified, Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::LastPlayed);
    songItem->setData(currentDetails.lastPlayed, Qt::UserRole);
    songItem=item(currentRow,KNMusicGlobal::Size);
    songItem->setData(currentDetails.size, Qt::UserRole);
    songItem->setData(QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    songItem=item(currentRow,KNMusicGlobal::Rating);
    songItem->setData(currentDetails.rating,
                      0);
    songItem->setEditable(true);
    songItem=item(currentRow,KNMusicGlobal::DateAdded);
    songItem->setText(KNGlobal::instance()->dateTimeToDisplayString(
                          songItem->data(Qt::UserRole).toDateTime()));
}
