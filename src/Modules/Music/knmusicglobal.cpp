#include <QDebug>

#include "knmusicglobal.h"

KNMusicGlobal *KNMusicGlobal::m_instance=nullptr;

KNMusicGlobal *KNMusicGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicGlobal:m_instance;
}

QString KNMusicGlobal::getGenre(const int &index)
{
    quint8 genreIndex=(quint8)index;
    if(genreIndex<m_genreList.count())
    {
        return m_genreList.at(genreIndex);
    }
    return QString("Unknown");
}

QString KNMusicGlobal::getGenre(const QString &value)
{
    if(value.count()<2)
    {
        //Ignore the bug.
        return value;
    }
    QString resultValue=value;
    if(resultValue.at(0)=='(')
    {
        if(resultValue.at(resultValue.count()-1)==')')
        {
            //Means it might be sth like: (24)
            resultValue=resultValue.mid(1, resultValue.length()-2);
            quint8 valueTest=(quint8)resultValue.toInt();
            if(valueTest<m_genreList.count())
            {
                return m_genreList.at(valueTest);
            }
            return resultValue;
        }
        int rightBracketIndex=resultValue.indexOf(")");
        if(rightBracketIndex>0)
        {
            resultValue=resultValue.mid(rightBracketIndex+1);
        }
        return resultValue;
    }
    return resultValue;
}

QString KNMusicGlobal::getHeader(const int &index)
{
    return m_header[index];
}

void KNMusicGlobal::retranslate()
{
    m_header[Name]=tr("Name");
    m_header[Album]=tr("Album");
    m_header[AlbumArtist]=tr("Album Artist");
    m_header[AlbumRating]=tr("Album Rating");
    m_header[Artist]=tr("Artist");
    m_header[BeatsPerMinuate]=tr("Beats Per Minuate");
    m_header[BitRate]=tr("Bit Rate");
    m_header[Category]=tr("Category");
    m_header[Comments]=tr("Comments");
    m_header[Composer]=tr("Composer");
    m_header[DateAdded]=tr("Date Added");
    m_header[DateModified]=tr("Date Modified");
    m_header[Description]=tr("Description");
    m_header[DiscNumber]=tr("Disc Number");
    m_header[EpisodeID]=tr("Episode ID");
    m_header[EpisodeNumber]=tr("Episode Number");
    m_header[Equalizer]=tr("Equalizer");
    m_header[Genre]=tr("Genre");
    m_header[Grouping]=tr("Grouping");
    m_header[Kind]=tr("Kind");
    m_header[LastPlayed]=tr("Last Played");
    m_header[LastSkipped]=tr("Last Skipped");
    m_header[Plays]=tr("Plays");
    m_header[Rating]=tr("Rating");
    m_header[SampleRate]=tr("Sample Rate");
    m_header[Season]=tr("Season");
    m_header[Show]=tr("Show");
    m_header[Size]=tr("Size");
    m_header[Skips]=tr("Skips");
    m_header[Time]=tr("Time");
    m_header[TrackNumber]=tr("Track Number");
    m_header[Year]=tr("Year");
}

void KNMusicGlobal::retranslateAndSet()
{
    retranslate();
}

KNMusicGlobal::KNMusicGlobal() :
    QObject()
{
    retranslate();
    qRegisterMetaType<KNMusicGlobal::MusicDetailsInfo>("KNMusicGlobal::MusicDetailsInfo");
    m_genreList<<"Blues"
               <<"Classic Rock"
               <<"Country"
               <<"Dance"
               <<"Disco"
               <<"Funk"
               <<"Grunge"
               <<"Hip-Hop"
               <<"Jazz"
               <<"Metal"
               <<"New Age"
               <<"Oldies"
               <<"Other"
               <<"Pop"
               <<"R&B"
               <<"Rap"
               <<"Reggae"
               <<"Rock"
               <<"Techno"
               <<"Industrial"
               <<"Alternative"
               <<"Ska"
               <<"Death Metal"
               <<"Pranks"
               <<"Soundtrack"
               <<"Euro-Techno"
               <<"Ambient"
               <<"Trip-Hop"
               <<"Vocal"
               <<"Jazz+Funk"
               <<"Fusion"
               <<"Trance"
               <<"Classical"
               <<"Instrumental"
               <<"Acid"
               <<"House"
               <<"Game"
               <<"Sound Clip"
               <<"Gospel"
               <<"Noise"
               <<"Alternative Rock"
               <<"Bass"
               <<"Soul"
               <<"Punk"
               <<"Space"
               <<"Meditative"
               <<"Instrumental Pop"
               <<"Instrumental Rock"
               <<"Ethnic"
               <<"Gothic"
               <<"Darkwave"
               <<"Techno-Industrial"
               <<"Electronic"
               <<"Pop-Folk"
               <<"Eurodance"
               <<"Dream"
               <<"Southern Rock"
               <<"Comedy"
               <<"Cult"
               <<"Gangsta"
               <<"Top 40"
               <<"Christian Rap"
               <<"Pop/Funk"
               <<"Jungle"
               <<"Native US"
               <<"Cabaret"
               <<"New Wave"
               <<"Psychadelic"
               <<"Rave"
               <<"Showtunes"
               <<"Trailer"
               <<"Lo-Fi"
               <<"Tribal"
               <<"Acid Punk"
               <<"Acid Jazz"
               <<"Polka"
               <<"Retro"
               <<"Musical"
               <<"Rock & Roll"
               <<"Hard Rock"
               <<"Folk"
               <<"Folk-Rock"
               <<"National Folk"
               <<"Swing"
               <<"Fast Fusion"
               <<"Bebob"
               <<"Latin"
               <<"Revival"
               <<"Celtic"
               <<"Bluegrass"
               <<"Avantgarde"
               <<"Gothic Rock"
               <<"Progressive Rock"
               <<"Psychedelic Rock"
               <<"Symphonic Rock"
               <<"Slow Rock"
               <<"Big Band"
               <<"Chorus"
               <<"Easy Listening"
               <<"Acoustic"
               <<"Humour"
               <<"Speech"
               <<"Chanson"
               <<"Opera"
               <<"Chamber Music"
               <<"Sonata"
               <<"Symphony"
               <<"Booty Bass"
               <<"Primus"
               <<"Porn Groove"
               <<"Satire"
               <<"Slow Jam"
               <<"Club"
               <<"Tango"
               <<"Samba"
               <<"Folklore"
               <<"Ballad"
               <<"Power Ballad"
               <<"Rhythmic Soul"
               <<"Freestyle"
               <<"Duet"
               <<"Punk Rock"
               <<"Drum Solo"
               <<"Acapella"
               <<"Euro-House"
               <<"Dance Hall"
               <<"Goa"
               <<"Drum & Bass"
               <<"Club House"
               <<"Hardcore"
               <<"Terror"
               <<"Indie"
               <<"BritPop"
               <<"Negerpunk"
               <<"Polsk Punk"
               <<"Beat"
               <<"Christian Gangsta Rap"
               <<"Heavy Metal"
               <<"Black Metal"
               <<"Crossover"
               <<"Contemporary Christian"
               <<"Christian Rock"
               <<"Merengue"
               <<"Salsa"
               <<"Thrash Metal"
               <<"Anime"
               <<"JPop"
               <<"Synthpop";
}
