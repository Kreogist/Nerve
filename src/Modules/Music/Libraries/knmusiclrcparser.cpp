#include <QFile>
#include <QTextStream>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QLinkedList>

#include <QDebug>

#include "knmusiclrcparser.h"

KNMusicLRCParser::KNMusicLRCParser(QObject *parent) :
    KNMusicLRCParserBase(parent)
{
    //Set frame catch regexp.
    m_frameCatch.setPattern("\\[[^\\]]*\\]");
    //Set the header text.
    m_headerText.append("ti");
    m_headerText.append("ar");
    m_headerText.append("al");
}

bool KNMusicLRCParser::readLyrics(QString filePath)
{
    //Open the lyric file.
    QFile lyricFile(filePath);
    if(!lyricFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QTextStream lyricReader(&lyricFile);
    //Clear the raw data cache and current cache.
    resetCache();
    /*Read the line one by one, in order to avoid the different line feed under
      Windows and UNIX/Linux.*/
    QString lyricsLineData;
    while(!(lyricsLineData=lyricReader.readLine()).isNull())
    {
        m_lyricsRawData.append(lyricsLineData);
    }
    //Close the file.
    lyricFile.close();
    //Parse raw data.
    parseLyrics();
    return true;
}

QString KNMusicLRCParser::title() const
{
    return m_title;
}

QString KNMusicLRCParser::artist() const
{
    return m_artist;
}

QString KNMusicLRCParser::album() const
{
    return m_album;
}

void KNMusicLRCParser::resetCache()
{
    m_lyricsRawData.clear();
    m_lyrics.clear();
    m_position.clear();
    m_lyricsText.clear();
}

void KNMusicLRCParser::parseLyrics()
{
    //Process each line.
    while(!m_lyricsRawData.isEmpty())
    {
        QString currentLine=m_lyricsRawData.takeFirst();
        //Remove all the spaces.
        currentLine=currentLine.simplified();
        //If the line is empty, process next.
        if(currentLine.isEmpty())
        {
            continue;
        }
        //Using a linked list to storage all the frames.
        QLinkedList<QString> currentFrames;
        //Using RegExp to catch the data.
        QRegularExpressionMatchIterator matchIterator=m_frameCatch.globalMatch(currentLine);
        int lastPos=0;
        while(matchIterator.hasNext())
        {
            QRegularExpressionMatch match=matchIterator.next();
            if(match.capturedStart()!=lastPos)
            {
                //Means it's not in the head of the current line.
                break;
            }
            currentFrames.append(currentLine.mid(match.capturedStart(), match.capturedLength()));
            lastPos=match.capturedEnd();
        }
        currentLine.remove(0, lastPos);
        while(!currentFrames.isEmpty())
        {
            parseFrame(currentFrames.takeFirst(), currentLine);
        }
    }
    qStableSort(m_lyrics.begin(), m_lyrics.end(), frameLessThan);
    for(int i=0; i<m_lyrics.size(); i++)
    {
        m_position.append(m_lyrics.at(i).position);
        m_lyricsText.append(m_lyrics.at(i).text);
    }
}

void KNMusicLRCParser::parseFrame(const QString &frame,
                                  const QString &lineData)
{
    //Remove the first '[' and ']'.
    QString frameData=frame.mid(1, frame.length()-2);
    //Get the data before the first colon, judge it's number or not.
    int colonPos=frameData.indexOf(':');
    if(colonPos==-1)
    {
        //Cannot find ':', this is frame is obsolete.
        return;
    }
    QString testData=frameData.left(colonPos).toLower();
    //Search header text in header, if has find.
    switch(m_headerText.indexOf(testData))
    {
    case TitleFrame:
        m_title=lineData;
        return;
    case ArtistFrame:
        m_artist=lineData;
        return;
    case AlbumFrame:
        m_album=lineData;
        return;
    case ByFrame:
        return;
    default:
        break;
    }
    //Judge the test data is number or not.
    int secondChar=frameData.indexOf(QRegularExpression("[^0-9]"), colonPos+1);
    if(secondChar==-1)
    {
        return;
    }
    lrcFrame currentFrame;
    currentFrame.position=testData.toFloat()*60.0+
                frameData.mid(colonPos+1, secondChar-colonPos-1).toFloat()+
                frameData.mid(secondChar+1).toFloat()/1000.0;
    currentFrame.text=lineData;
    m_lyrics.append(currentFrame);
}

bool KNMusicLRCParser::frameLessThan(const lrcFrame &frameLeft,
                                     const lrcFrame &frameRight)
{
    return frameLeft.position<frameRight.position;
}

QStringList KNMusicLRCParser::lyricsTexts() const
{
    return m_lyricsText;
}

QList<float> KNMusicLRCParser::positions() const
{
    return m_position;
}
