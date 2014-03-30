#include <QList>
#include <QStandardItem>

#include <QDebug>

#include "../../knlocale.h"
#include "knmusicstarrating.h"

#include "knmusicmodel.h"

KNMusicModel::KNMusicModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();
    instance=KNMusicGlobal::instance();
    QStringList header;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        header<<(instance->getHeader(i));
    }
    setHorizontalHeaderLabels(header);

    setHeaderData(KNMusicGlobal::TrackNumber, Qt::Horizontal, 1, Qt::UserRole);
    setHeaderData(KNMusicGlobal::Size, Qt::Horizontal, 2, Qt::UserRole);

    connect(KNLocale::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));
}

void KNMusicModel::resetHeader()
{
    emit requireResetHeader();
}

QString KNMusicModel::filePathFromIndex(const QModelIndex &index)
{
    return item(index.row(), KNMusicGlobal::Name)->data(Qt::UserRole).toString();
}

void KNMusicModel::appendMusic(const QStringList &info,
                               const KNMusicGlobal::MusicDetailsInfo &datas)
{
    QList<QStandardItem *> musicItem;
    for(int i=0;
        i<info.count();
        i++)
    {
        QStandardItem *infoItem=new QStandardItem(info.at(i));
        infoItem->setEditable(false);
        musicItem.append(infoItem);
    }
    QStandardItem *dataItem=musicItem.at(KNMusicGlobal::Time);
    dataItem->setData(datas.coverImage, Qt::UserRole);
    dataItem=musicItem.at(KNMusicGlobal::DateModified);
    dataItem->setData(datas.dateModified, Qt::UserRole);
    dataItem=musicItem.at(KNMusicGlobal::LastPlayed);
    dataItem->setData(datas.lastPlayed, Qt::UserRole);
    dataItem=musicItem.at(KNMusicGlobal::Size);
    dataItem->setData(datas.size, Qt::UserRole);
    dataItem=musicItem.at(KNMusicGlobal::Rating);
    dataItem->setData(QVariant::fromValue(KNMusicStarRating(datas.rating)),
                      0);
    dataItem->setEditable(true);

    dataItem=musicItem.at(KNMusicGlobal::Name);
    dataItem->setData(datas.filePath, Qt::UserRole);
    appendRow(musicItem);
    emit musicAppend(indexFromItem(dataItem));
}

void KNMusicModel::retranslate()
{
    ;
}

void KNMusicModel::retranslateAndSet()
{
    retranslate();
}
