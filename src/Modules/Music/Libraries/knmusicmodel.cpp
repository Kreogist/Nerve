#include <QList>
#include <QStandardItem>

#include <QDebug>

#include "../../knlocale.h"

#include "knmusicmodel.h"

KNMusicModel::KNMusicModel(QObject *parent) :
    QStandardItemModel(parent)
{
    retranslate();
    QStringList header;
    for(int i=0;i<KNMusicGlobal::MusicDataCount;i++)
    {
        header<<m_header[i];
    }
    setHorizontalHeaderLabels(header);

    connect(KNLocale::instance(), SIGNAL(languageChanged()),
            this, SLOT(retranslateAndSet()));
}

void KNMusicModel::resetHeader()
{
    emit requireResetHeader();
}

void KNMusicModel::appendMusic(const QStringList &info,
                               const QPixmap &coverImage)
{
    QList<QStandardItem *> musicItem;
    for(int i=0;
        i<info.count();
        i++)
    {
        QStandardItem *infoItem=new QStandardItem(info.at(i));
        musicItem.append(infoItem);
    }
    QStandardItem *dataItem=musicItem.at(0);
    dataItem->setData(coverImage);
    appendRow(musicItem);
}

void KNMusicModel::retranslate()
{
    m_header[KNMusicGlobal::Name]=tr("Name");
    m_header[KNMusicGlobal::Album]=tr("Album");
    m_header[KNMusicGlobal::AlbumArtist]=tr("Album Artist");
    m_header[KNMusicGlobal::AlbumRating]=tr("Album Rating");
    m_header[KNMusicGlobal::Artist]=tr("Artist");
    m_header[KNMusicGlobal::BeatsPerMinuate]=tr("Beats Per Minuate");
    m_header[KNMusicGlobal::BitRate]=tr("Bit Rate");
    m_header[KNMusicGlobal::Category]=tr("Category");
    m_header[KNMusicGlobal::Comments]=tr("Comments");
    m_header[KNMusicGlobal::Composer]=tr("Composer");
    m_header[KNMusicGlobal::DateAdded]=tr("Date Added");
    m_header[KNMusicGlobal::DateModified]=tr("Date Modified");
    m_header[KNMusicGlobal::Description]=tr("Description");
    m_header[KNMusicGlobal::DiscNumber]=tr("Disc Number");
    m_header[KNMusicGlobal::EpisodeID]=tr("Episode ID");
    m_header[KNMusicGlobal::EpisodeNumber]=tr("Episode Number");
    m_header[KNMusicGlobal::Equalizer]=tr("Equalizer");
    m_header[KNMusicGlobal::Genre]=tr("Genre");
    m_header[KNMusicGlobal::Grouping]=tr("Grouping");
    m_header[KNMusicGlobal::Kind]=tr("Kind");
    m_header[KNMusicGlobal::LastPlayed]=tr("Last Played");
    m_header[KNMusicGlobal::LastSkipped]=tr("Last Skipped");
    m_header[KNMusicGlobal::Plays]=tr("Plays");
    m_header[KNMusicGlobal::PurchaseDate]=tr("Purchase Date");
    m_header[KNMusicGlobal::Rating]=tr("Rating");
    m_header[KNMusicGlobal::ReleaseData]=tr("Release Data");
    m_header[KNMusicGlobal::SampleRate]=tr("Sample Rate");
    m_header[KNMusicGlobal::Season]=tr("Season");
    m_header[KNMusicGlobal::Show]=tr("Show");
    m_header[KNMusicGlobal::Size]=tr("Size");
    m_header[KNMusicGlobal::Skips]=tr("Skips");
    m_header[KNMusicGlobal::Time]=tr("Time");
    m_header[KNMusicGlobal::TrackNumber]=tr("Track Number");
    m_header[KNMusicGlobal::Year]=tr("Year");
}

void KNMusicModel::retranslateAndSet()
{
    retranslate();
}
