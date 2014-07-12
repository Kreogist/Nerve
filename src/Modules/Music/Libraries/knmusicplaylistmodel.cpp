#include "knmusicplaylistmodel.h"

KNMusicPlaylistModel::KNMusicPlaylistModel(QObject *parent) :
    KNMusicModelBase(parent)
{
    resetHeader();
}

void KNMusicPlaylistModel::resetHeader()
{
    //Set the header text.
    QStringList header;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        header<<(m_musicGlobal->getHeader(i));
    }
    //Append a space for playlist ID.
    header<<" ";
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
