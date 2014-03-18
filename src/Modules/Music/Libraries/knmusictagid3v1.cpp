#include <QDataStream>
#include <QFile>
#include <QTextCodec>

#include <QDebug>

#include <cstdlib>

#include "knmusictagid3v1.h"

KNMusicTagID3v1::KNMusicTagID3v1(QObject *parent) :
    KNMusicTagBase(parent)
{
    m_genreList.clear();
    m_codec=QTextCodec::codecForName("GBK");
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

bool KNMusicTagID3v1::readTag(const QString &filePath)
{
    m_tagData.title.clear();
    m_tagData.artist.clear();
    m_tagData.album.clear();
    m_tagData.year.clear();
    m_tagData.comment.clear();
    m_tagData.track=0;
    m_tagData.genre.clear();

    //Get raw tag data
    QFile mediaFile(filePath);
    if(mediaFile.size()<128)
    {
        //If the size is less than 128, it can't contains ID3v1.
        return false;
    }
    if(!mediaFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QDataStream mediaData(&mediaFile);
    mediaData.skipRawData(mediaFile.size()-128);
    char rawTagData[128];
    mediaData.readRawData(rawTagData, 128);
    mediaFile.close();

    if(rawTagData[0]!='T' || rawTagData[1]!='A' || rawTagData[2]!='G')
    {
        //Can't find 'TAG'.
        return false;
    }
    char rawText[30];
    strncpy(rawText, rawTagData+3, 30);
    m_tagData.title=QString::fromLocal8Bit(rawText).simplified();
    strncpy(rawText, rawTagData+33, 30);
    m_tagData.artist=QString::fromLocal8Bit(rawText).simplified();
    strncpy(rawText, rawTagData+63, 30);
    m_tagData.album=QString::fromLocal8Bit(rawText).simplified();
    strncpy(rawText, rawTagData+93, 4);
    rawText[4]='\0';
    m_tagData.year=QString::fromLocal8Bit(rawText).simplified();
    if(rawTagData[125]==0)
    {
        m_tagData.track=(int)rawTagData[126];
        strncpy(rawText, rawTagData+97, 28);
        m_tagData.comment=QString::fromLocal8Bit(rawText).simplified();
    }
    else
    {
        m_tagData.track=0;
        strncpy(rawText, rawTagData+97, 30);
        m_tagData.comment=QString::fromLocal8Bit(rawText).simplified();
    }
    m_tagData.genre=getGenre((int)rawTagData[127]);
    return true;
}

QString KNMusicTagID3v1::getGenre(const int &index)
{
    quint8 genreIndex=(quint8)index;
    if(genreIndex<m_genreList.count())
    {
        return m_genreList.at(genreIndex);
    }
    return QString("Unknown");
}

KNMusicTagID3v1::ID3v1Data KNMusicTagID3v1::tagData() const
{
    return m_tagData;
}

void KNMusicTagID3v1::setTagData(const KNMusicTagID3v1::ID3v1Data &tagData)
{
    m_tagData=tagData;
}
