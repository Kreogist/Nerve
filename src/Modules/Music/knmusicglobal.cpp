#include <QDebug>

#include <QVector>

#include "knmusicglobal.h"

KNMusicGlobal *KNMusicGlobal::m_instance=nullptr;

KNMusicGlobal *KNMusicGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicGlobal:m_instance;
}

QString KNMusicGlobal::getGenre(const int &index) const
{
    quint8 genreIndex=(quint8)index;
    if(genreIndex<m_genreList.size())
    {
        return m_genreList.at(genreIndex);
    }
    return QString("Unknown");
}

QString KNMusicGlobal::getGenre(const QString &value) const
{
    if(value.size()<2)
    {
        //Ignore the bug.
        return value;
    }
    QString resultValue=qPrintable(QString(value));
    if(resultValue.at(0)=='(')
    {
        int rightBracketIndex=resultValue.indexOf(')');
        if(rightBracketIndex!=-1)
        {
            //Means it might be sth like: (24)
            quint8 valueTest=(quint8)resultValue.mid(1, rightBracketIndex-1).toInt();
            return valueTest<m_genreList.size()?
                   m_genreList.at(valueTest):
                   resultValue.mid(rightBracketIndex+1);

        }
    }
    return resultValue;
}

QPixmap KNMusicGlobal::getGenreImage(const QString &value) const
{
    return QPixmap(":/MusicGenre/Resources/Music/genres/" +
                   m_genreImage[value.toLower()]);
}

QString KNMusicGlobal::getHeader(const int &index)
{
    return m_header[index];
}

int KNMusicGlobal::getMusicType(const QString &suffix) const
{
    return m_suffixs.indexOf(suffix.toLower());
}

QString KNMusicGlobal::getDescription(const int &type) const
{
    return m_suffixDescription.at(type);
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
    m_header[DiscCount]=tr("Disc Count");
    m_header[DiscNumber]=tr("Disc Number");
    m_header[Equalizer]=tr("Equalizer");
    m_header[Genre]=tr("Genre");
    m_header[Grouping]=tr("Grouping");
    m_header[Kind]=tr("Kind");
    m_header[LastPlayed]=tr("Last Played");
    m_header[Rating]=tr("Rating");
    m_header[SampleRate]=tr("Sample Rate");
    m_header[Size]=tr("Size");
    m_header[Time]=tr("Time");
    m_header[TrackCount]=tr("Track Count");
    m_header[TrackNumber]=tr("Track Number");
    m_header[Year]=tr("Year");

    m_suffixDescription.clear();
    m_suffixDescription<<tr("MPEG Audio Layer III (mp3)")
                       <<tr("MPEG-4 Part 14 (m4a)")
                       <<tr("Waveform Audio File Format (wav)")
                       <<tr("Free Lossless Audio Codec (flac)")
                       <<tr("Monkey's Audio (ape)")
                       <<tr("Ogg Vorbis Audio (ogg)")
                       <<tr("True Audio Codec (tta)")
                       <<tr("Audio Interchange File Format (aiff)")
                       <<tr("Audio Interchange File Format (aifc)")
                       <<tr("Audio Interchange File Format (aif)")
                       <<tr("MPEG-4 Part 14 (mp4)")
                       <<tr("MPEG Audio Layer II (mpa)")
                       <<tr("MPEG Audio Layer II (mp2)")
                       <<tr("MPEG Audio Layer I (mp1)")
                       <<tr("Musical Instrument Digital Interface (midi)")
                       <<tr("Musical Instrument Digital Interface (mid)")
                       <<tr("MPEG Audio Layer III with SBR (mp3pro)")
                       <<tr("Musepack Lossy Audio Codec (mpc)")
                       <<tr("Advanced Audio Coding (aac)")
                       <<tr("Compact Disc Audio track (cda)")
                       <<tr("Windows Media Audio (wma)")
                       <<tr("Flash Audio (fla)")
                       <<tr("Tom's lossless Audio Kompressor (tak)")
                       <<tr("Musepack Lossy Audio Codec (mp+)")
                       <<tr("Audible Audio File (aa)")
                       <<tr("Real Audio (ra)")
                       <<tr("Monkey's Audio (mac)")
                       <<tr("Musical Instrument Digital Interface (rmi)")
                       <<tr("DTS Audio Codec (dtswav)")
                       <<tr("DTS Audio Codec (dts)")
                       <<tr("SouND Audio (snd)")
                       <<tr("Au File Format (au)")
                       <<tr("Dolby Surround Audio Coding-3 (ac3)")
                       <<tr("Fast Tracker Module (xm)")
                       <<tr("Unreal Engine 1 Music Format (umx)");
}

void KNMusicGlobal::retranslateAndSet()
{
    retranslate();
}

KNMusicGlobal::KNMusicGlobal() :
    QObject()
{
    retranslate();
    m_noAlbumImage=QImage(":/Music/Resources/Music/noalbum.png");
    //Don't touch this. I don't why, but it works.
    qRegisterMetaType<KNMusicGlobal::MusicDetailsInfo>("KNMusicGlobal::MusicDetailsInfo");
    qRegisterMetaType<QVector<int>>("QVector<int>");
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

    m_genreImage["blues"]="genre-blues.jpg";
    m_genreImage["classic rock"]="Classic Rock";
    m_genreImage["country"]="genre-country.jpg";
    m_genreImage["dance"]="genre-dance.jpg";
    m_genreImage["disco"]="Disco";
    m_genreImage["funk"]="Funk";
    m_genreImage["grunge"]="Grunge";
    m_genreImage["hip-hop"]="genre-hiphop.jpg";
    m_genreImage["jazz"]="genre-jazz.jpg";
    m_genreImage["metal"]="Metal";
    m_genreImage["new age"]="New Age";
    m_genreImage["oldies"]="Oldies";
    m_genreImage["other"]="Other";
    m_genreImage["pop"]="genre-pop.jpg";
    m_genreImage["r&b"]="genre-rb.jpg";
    m_genreImage["rap"]="Rap";
    m_genreImage["reggae"]="Reggae";
    m_genreImage["rock"]="genre-rock.jpg";
    m_genreImage["techno"]="Techno";
    m_genreImage["industrial"]="Industrial";
    m_genreImage["alternative"]="genre-alternative.jpg";
    m_genreImage["ska"]="Ska";
    m_genreImage["death metal"]="Death Metal";
    m_genreImage["pranks"]="Pranks";
    m_genreImage["soundtrack"]="genre-soundtrack.jpg";
    m_genreImage["euro-techno"]="Euro-Techno";
    m_genreImage["ambient"]="Ambient";
    m_genreImage["trip-hop"]="Trip-Hop";
    m_genreImage["vocal"]="genre-vocal.jpg";
    m_genreImage["jazz+funk"]="Jazz+Funk";
    m_genreImage["fusion"]="Fusion";
    m_genreImage["trance"]="Trance";
    m_genreImage["classical"]="genre-classical.jpg";
    m_genreImage["instrumental"]="Instrumental";
    m_genreImage["acid"]="Acid";
    m_genreImage["house"]="House";
    m_genreImage["game"]="Game";
    m_genreImage["sound clip"]="Sound Clip";
    m_genreImage["gospel"]="Gospel";
    m_genreImage["noise"]="Noise";
    m_genreImage["alternative rock"]="Alternative Rock";
    m_genreImage["bass"]="Bass";
    m_genreImage["soul"]="Soul";
    m_genreImage["punk"]="Punk";
    m_genreImage["space"]="Space";
    m_genreImage["meditative"]="Meditative";
    m_genreImage["instrumental pop"]="Instrumental Pop";
    m_genreImage["instrumental rock"]="Instrumental Rock";
    m_genreImage["ethnic"]="Ethnic";
    m_genreImage["gothic"]="Gothic";
    m_genreImage["darkwave"]="Darkwave";
    m_genreImage["techno-industrial"]="Techno-Industrial";
    m_genreImage["electronic"]="genre-electronic.jpg";
    m_genreImage["pop-folk"]="genre-folk.jpg";
    m_genreImage["eurodance"]="genre-dance.jpg";
    m_genreImage["dream"]="Dream";
    m_genreImage["southern rock"]="Southern Rock";
    m_genreImage["comedy"]="Comedy";
    m_genreImage["cult"]="Cult";
    m_genreImage["gangsta"]="Gangsta";
    m_genreImage["top 40"]="Top 40";
    m_genreImage["christian rap"]="Christian Rap";
    m_genreImage["pop/funk"]="genre-pop.jpg";
    m_genreImage["jungle"]="Jungle";
    m_genreImage["native us"]="Native US";
    m_genreImage["cabaret"]="Cabaret";
    m_genreImage["new wave"]="New Wave";
    m_genreImage["psychadelic"]="Psychadelic";
    m_genreImage["rave"]="Rave";
    m_genreImage["showtunes"]="Showtunes";
    m_genreImage["trailer"]="Trailer";
    m_genreImage["lo-fi"]="Lo-Fi";
    m_genreImage["tribal"]="Tribal";
    m_genreImage["acid punk"]="Acid Punk";
    m_genreImage["acid jazz"]="Acid Jazz";
    m_genreImage["polka"]="Polka";
    m_genreImage["retro"]="Retro";
    m_genreImage["musical"]="genre-music.jpg";
    m_genreImage["rock & roll"]="Rock & Roll";
    m_genreImage["hard rock"]="Hard Rock";
    m_genreImage["folk"]="genre-folk.jpg";
    m_genreImage["folk-rock"]="Folk-Rock";
    m_genreImage["national folk"]="National Folk";
    m_genreImage["swing"]="Swing";
    m_genreImage["fast fusion"]="Fast Fusion";
    m_genreImage["bebob"]="Bebob";
    m_genreImage["latin"]="Latin";
    m_genreImage["revival"]="Revival";
    m_genreImage["celtic"]="Celtic";
    m_genreImage["bluegrass"]="Bluegrass";
    m_genreImage["avantgarde"]="Avantgarde";
    m_genreImage["gothic rock"]="Gothic Rock";
    m_genreImage["progressive rock"]="Progressive Rock";
    m_genreImage["psychedelic rock"]="Psychedelic Rock";
    m_genreImage["symphonic rock"]="Symphonic Rock";
    m_genreImage["slow rock"]="Slow Rock";
    m_genreImage["big band"]="Big Band";
    m_genreImage["chorus"]="Chorus";
    m_genreImage["easy listening"]="Easy Listening";
    m_genreImage["acoustic"]="Acoustic";
    m_genreImage["humour"]="Humour";
    m_genreImage["speech"]="Speech";
    m_genreImage["chanson"]="Chanson";
    m_genreImage["opera"]="Opera";
    m_genreImage["chamber music"]="Chamber Music";
    m_genreImage["sonata"]="Sonata";
    m_genreImage["symphony"]="Symphony";
    m_genreImage["booty bass"]="Booty Bass";
    m_genreImage["primus"]="Primus";
    m_genreImage["porn groove"]="Porn Groove";
    m_genreImage["satire"]="Satire";
    m_genreImage["slow jam"]="Slow Jam";
    m_genreImage["club"]="Club";
    m_genreImage["tango"]="Tango";
    m_genreImage["samba"]="Samba";
    m_genreImage["folklore"]="Folklore";
    m_genreImage["ballad"]="Ballad";
    m_genreImage["power ballad"]="Power Ballad";
    m_genreImage["rhythmic soul"]="Rhythmic Soul";
    m_genreImage["freestyle"]="Freestyle";
    m_genreImage["duet"]="Duet";
    m_genreImage["punk rock"]="Punk Rock";
    m_genreImage["drum solo"]="Drum Solo";
    m_genreImage["acapella"]="Acapella";
    m_genreImage["euro-house"]="Euro-House";
    m_genreImage["dance hall"]="Dance Hall";
    m_genreImage["goa"]="Goa";
    m_genreImage["drum & bass"]="Drum & Bass";
    m_genreImage["club house"]="Club House";
    m_genreImage["hardcore"]="Hardcore";
    m_genreImage["terror"]="Terror";
    m_genreImage["indie"]="Indie";
    m_genreImage["britpop"]="BritPop";
    m_genreImage["negerpunk"]="Negerpunk";
    m_genreImage["polsk punk"]="Polsk Punk";
    m_genreImage["beat"]="Beat";
    m_genreImage["christian gangsta rap"]="Christian Gangsta Rap";
    m_genreImage["heavy metal"]="Heavy Metal";
    m_genreImage["black metal"]="Black Metal";
    m_genreImage["crossover"]="Crossover";
    m_genreImage["contemporary christian"]="Contemporary Christian";
    m_genreImage["christian rock"]="Christian Rock";
    m_genreImage["merengue"]="Merengue";
    m_genreImage["salsa"]="Salsa";
    m_genreImage["thrash metal"]="Thrash Metal";
    m_genreImage["anime"]="genre-animation.jpg";
    m_genreImage["animation"]="genre-animation.jpg";
    m_genreImage["j-pop"]="genre-jpop.jpg";
    m_genreImage["jpop"]="genre-jpop.jpg";
    m_genreImage["synthpop"]="Synthpop";

    m_suffixs<<"mp3"
             <<"m4a"
             <<"wav"
             <<"flac"
             <<"ape"
             <<"ogg"
             <<"tta"
             <<"aiff"
             <<"aifc"
             <<"aif"
             <<"mp4"
             <<"mpa"
             <<"mp2"
             <<"mp1"
             <<"midi"
             <<"mid"
             <<"mp3pro"
             <<"mpc"
             <<"aac"
             <<"cda"
             <<"wma"
             <<"fla"
             <<"tak"
             <<"mp+"
             <<"aa"
             <<"ra"
             <<"mac"
             <<"rmi"
             <<"dtswav"
             <<"dts"
             <<"snd"
             <<"au"
             <<"ac3"
             <<"xm"
             <<"umx";
}

QImage KNMusicGlobal::noAlbumImage() const
{
    return m_noAlbumImage;
}
