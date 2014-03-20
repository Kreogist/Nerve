#ifndef KNMUSICTAGWMA_H
#define KNMUSICTAGWMA_H

#include "knmusictagbase.h"

class QTextCodec;
class KNMusicTagWma : public KNMusicTagBase
{
    Q_OBJECT
public:
    explicit KNMusicTagWma(QObject *parent = 0);

    bool readTag(const QString &filePath);

signals:

public slots:

private:
    unsigned char m_headerMark[17]={0x30, 0x26, 0xB2, 0x75,
                                    0x8E, 0x66, 0xCF, 0x11,
                                    0xA6, 0xD9, 0x00, 0xAA,
                                    0x00, 0x62, 0xCE, 0x6C};
    unsigned char m_standardFrame[17]={0x33, 0x26, 0xB2, 0x75,
                                       0x8E, 0x66, 0xCF, 0x11,
                                       0xA6, 0xD9, 0x00, 0xAA,
                                       0x00, 0x62, 0xCE, 0x6C};
    unsigned char m_extendedFrame[17]={0x40, 0xA4, 0xD0, 0xD2,
                                       0x07, 0xE3, 0xD2, 0x11,
                                       0x97, 0xF0, 0x00, 0xA0,
                                       0xC9, 0x5E, 0xA8, 0x50};
    enum WMA_FRAMEID_T
    {
        WMA_FRAMEID_AUTHOR = 0,
        WMA_FRAMEID_COPYRIGHT,
        WMA_FRAMEID_DESCRIPTION,
        WMA_FRAMEID_RATING,
        WMA_FRAMEID_TITLE,

        WMA_FRAMEID_ALBUMARTIST,
        WMA_FRAMEID_ALBUMCOVERURL,
        WMA_FRAMEID_ALBUMTITLE,
        WMA_FRAMEID_AUDIOFILEURL,
        WMA_FRAMEID_AUDIOSOURCEURL,
        WMA_FRAMEID_AUTHORURL,
        WMA_FRAMEID_BPMT,       // WM/BeatsPerMinute
        WMA_FRAMEID_CATEGORY,
        WMA_FRAMEID_COMPOSER,
        WMA_FRAMEID_CONDUCTOR,
        WMA_FRAMEID_CONTENTDISTRIBUTOR,
        WMA_FRAMEID_CGDPN, // WM/ContentGroupDescription
        WMA_FRAMEID_DIRECTOR,
        WMA_FRAMEID_DVDID,
        WMA_FRAMEID_ENCODEDBY,
        WMA_FRAMEID_ENCODINGSETTINGS,
        WMA_FRAMEID_ENCODINGTIME,
        WMA_FRAMEID_GENRE,
        WMA_FRAMEID_GENREID,
        WMA_FRAMEID_INITIALKEY,
        WMA_FRAMEID_ISRC,
        WMA_FRAMEID_LANGUAGE,
        WMA_FRAMEID_LYRICS,
        WMA_FRAMEID_LYRICSSYNC, // WM/Lyrics_Synchronised
        WMA_FRAMEID_MCDI,
        WMA_FRAMEID_MCPID, // WM/MediaClassPrimaryID
        WMA_FRAMEID_MCSID, // WM/MediaClassSecondaryID
        WMA_FRAMEID_MDACREDITS, // WM/MediaCredits
        WMA_FRAMEID_MDAISDELAY,   // WM/MediaIsDelay
        WMA_FRAMEID_MDAISFINALE,  // WM/MediaIsFinale
        WMA_FRAMEID_MDAISLIVE,    // WM/MediaIsLive
        WMA_FRAMEID_MDAISPREMIERE,  // WM/MediaIsPremiere
        WMA_FRAMEID_MDAISREPEAT,    // WM/MediaIsRepeat
        WMA_FRAMEID_MDAISSAP,       // WM/MediaIsSAP
        WMA_FRAMEID_MDAISSUBTITLED,  // WM/MediaIsSubtitled
        WMA_FRAMEID_MDAISSTEREO,     // WM/MediaIsStereo
        WMA_FRAMEID_MDANETWORKAFFILIATION,  // WM/MediaNetworkAffiliation
        WMA_FRAMEID_MDAORIGINALBDT,         // WM/MediaOriginalBroadcastDateTime
        WMA_FRAMEID_MDAORIGINALCHANNEL,     // WM/MediaOriginalChannel
        WMA_FRAMEID_MDASTATIONCALLSIGN,     // WM/MediaStationCallSign
        WMA_FRAMEID_MDASTATIONNAME,         // WM/MediaStationName
        WMA_FRAMEID_MODIFIEDBY,
        WMA_FRAMEID_MOOD,
        WMA_FRAMEID_ORIGALBUMTITLE,     // WM/OriginalAlbumTitle
        WMA_FRAMEID_ORIGARTIST,         // WM/OriginalArtist
        WMA_FRAMEID_ORIGFILENAME,       // WM/OriginalFilename
        WMA_FRAMEID_ORIGLYRICIST,       // WM/OriginalLyricist
        WMA_FRAMEID_ORIGRELEASETIME,    // WM/OriginalReleaseTime
        WMA_FRAMEID_ORIGRELEASEYEAR,    // WM/OriginalReleaseYear
        WMA_FRAMEID_PARENTALRATING,
        WMA_FRAMEID_PARENTALRATINGREASON,    // WM/ParentalRatingReason
        WMA_FRAMEID_PARTOFSET,
        WMA_FRAMEID_PERIOD,
        WMA_FRAMEID_PICTURE,
        WMA_FRAMEID_PRODUCER,
        WMA_FRAMEID_PROMOTIONURL,
        WMA_FRAMEID_PROVIDER,
        WMA_FRAMEID_PROVIDERCPYRIGHT,
        WMA_FRAMEID_PROVIDERRATING,
        WMA_FRAMEID_PROVIDERSTYLE,
        WMA_FRAMEID_PUBLISHER,
        WMA_FRAMEID_RADIOSTATIONNAME,
        WMA_FRAMEID_RADIOSTATIONOWNER,
        WMA_FRAMEID_SHAREUSERRATING,
        WMA_FRAMEID_SUBTITLE,
        WMA_FRAMEID_SUBTITLEDESCRIPTION,
        WMA_FRAMEID_TEXT,
        WMA_FRAMEID_TOOLNAME,
        WMA_FRAMEID_TOOLVERSION,
        WMA_FRAMEID_TRACK,
        WMA_FRAMEID_TRACKNUMBER,
        WMA_FRAMEID_UNIQUEFILEIDENTIFIER,
        WMA_FRAMEID_USERWEBURL,
        WMA_FRAMEID_WMCOLLECTIONGROUPID,
        WMA_FRAMEID_WMCOLLECTIONID,
        WMA_FRAMEID_CONTENTID,
        WMA_FRAMEID_WRITER,
        WMA_FRAMEID_YEAR,
        WMA_FRAMEID_SDKVERSION,
        WMA_FRAMEID_SDKNEEDED,
        WMA_FRAMEID_ISVBR,
        WMA_FRAMEID_UNKNOWN,
        WMAFrameCount
    };
    QString m_wmaTags[WMAFrameCount];
    QTextCodec *m_utf16Codec;

    void resetCache();
};

#endif // KNMUSICTAGWMA_H
