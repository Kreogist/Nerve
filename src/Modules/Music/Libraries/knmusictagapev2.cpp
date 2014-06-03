#include <QFile>

#include <QDebug>

#include <cstdlib>
#include <cstring>

#include "knmusictagapev2.h"

KNMusicTagAPEv2::KNMusicTagAPEv2(QObject *parent) :
    KNMusicTagBase(parent)
{
    //Set the frame of the index.
    m_frames[Name    ]="TITLE";
    m_frames[Artist  ]="ARTIST";
    m_frames[Album   ]="ALBUM";
    m_frames[Comments]="COMMENT";
    m_frames[Composer]="COMPOSER";
    m_frames[Genre   ]="GENRE";
    m_frames[Year    ]="YEAR";
    m_frames[Track   ]="TRACK";
}

QString KNMusicTagAPEv2::textData(const int &key) const
{
    int frameIndex=m_keys.indexOf(m_frames[key]);
    return frameIndex==-1?
                QString():QString(m_data.at(frameIndex)).simplified();
}

QString KNMusicTagAPEv2::frameAt(const int &key) const
{
    return m_frames[key];
}

QString KNMusicTagAPEv2::frameData(const QString &frame) const
{
    int frameIndex=m_keys.indexOf(frame);
    return frameIndex==-1?
                QString():QString(m_data.at(frameIndex)).simplified();
}

bool KNMusicTagAPEv2::readTag(const QFile &mediaFile,
                              QDataStream &mediaData)
{
    clearCache();
    if(mediaFile.size()<32)
    {
        //A file even can't contains a header.
        return false;
    }
    m_headerPosition=0;
    if(checkHeaderAndParse(mediaData))
    {
        m_headerPosition+=32;
        return readTagAt(mediaData);
    }
    m_headerPosition=mediaFile.size()-32;
    if(checkHeaderAndParse(mediaData))
    {
        m_headerPosition-=(m_tagSize-32);
        return readTagAt(mediaData);
    }
    m_headerPosition=mediaFile.size()-160;
    if(checkHeaderAndParse(mediaData))
    {
        m_headerPosition-=(m_tagSize-32);
        return readTagAt(mediaData);
    }
    m_headerPosition=-1;
    return false;
}

void KNMusicTagAPEv2::clearCache()
{
    memset(m_apeHeader, 0, 32);
    m_keys.clear();
    m_data.clear();
}

QStringList KNMusicTagAPEv2::keyList() const
{
    return m_keys;
}

void KNMusicTagAPEv2::setTextData(const int &key, const QString &data)
{
    setTextData(m_frames[key], data);
}

void KNMusicTagAPEv2::setTextData(const QString &key, const QString &data)
{
    int keyIndex=m_keys.indexOf(key);
    if(keyIndex==-1)
    {
        m_keys.append(key);
        m_data.append(data.toUtf8());
    }
    else
    {
        m_data.replace(keyIndex, data.toUtf8());
    }
}

void KNMusicTagAPEv2::writeTag(QFile &mediaFile, QDataStream &mediaData)
{
    //Get the item numbers.
    int itemCounts=m_keys.size();
    char sizeData[4], flags[4];
    quint32 itemSize;
    QByteArray tagContentData;
    for(int i=0; i<itemCounts; i++)
    {
        //Create every item frame.
        QByteArray currentItem;
        //Begin the item is the frame.
        itemSize=m_data.at(i).size();
        sizeData[0]=(quint8)( itemSize&0b00000000000000000000000011111111     );
        sizeData[1]=(quint8)((itemSize&0b00000000000000001111111100000000)>>8 );
        sizeData[2]=(quint8)((itemSize&0b00000000111111110000000000000000)>>16);
        sizeData[3]=(quint8)( itemSize                                    >>24);
        currentItem.append(sizeData, 4);
        //Next the flag of the item.(I cheated)
        flags[0]=0;flags[1]=0;flags[2]=0;flags[3]=0;
        currentItem.append(flags, 4);
        //Then the key of the item, append a '\0'.
        currentItem.append(m_keys.at(i).toUtf8());
        currentItem.append('\0');
        //Finally, the data of the frame.
        currentItem.append(m_data.at(i));
        //Append current frame data to content.
        tagContentData.append(currentItem);
    }
    //Create the header of the tag.
    char tagHeader[32];
    strcpy(tagHeader, m_apePreamble);
    //Set the version(Another cheat, I force to set to APEv2)
    tagHeader[8]=0xD0;tagHeader[9]=0x07;
    tagHeader[10]=0x00;tagHeader[11]=0x00;
    //Set tag content sizes.
    //IMPORTANT: This value should + 32 if contains footer.
    quint32 tagContentSize=tagContentData.size()+32;
    tagHeader[12]=(quint8)( tagContentSize&0b00000000000000000000000011111111     );
    tagHeader[13]=(quint8)((tagContentSize&0b00000000000000001111111100000000)>>8 );
    tagHeader[14]=(quint8)((tagContentSize&0b00000000111111110000000000000000)>>16);
    tagHeader[15]=(quint8)( tagContentSize                                    >>24);
    //Item count.
    tagHeader[16]=(quint8)( itemCounts&0b00000000000000000000000011111111     );
    tagHeader[17]=(quint8)((itemCounts&0b00000000000000001111111100000000)>>8 );
    tagHeader[18]=(quint8)((itemCounts&0b00000000111111110000000000000000)>>16);
    tagHeader[19]=(quint8)( itemCounts                                    >>24);
    //Set the flags.(Last cheat, I force set to:)
    // 0b00000000 0b00000000 0b00000000 0b00000101
    //   0      7   8     15  16     23  24     31
    //It means: It contains a header, a footer. At the header, #29 will be 1.
    tagHeader[20]=0b00000000;
    tagHeader[21]=0b00000000;
    tagHeader[22]=0b00000000;
    tagHeader[23]=0b00000101;
    //Reserved data, must be zero.
    for(int i=24; i<32; i++)
    {
        tagHeader[i]=0x00;
    }
    int headerPos=0;
    quint32 originalTagSize, realDataSize;
    char headerTester[9], headerCache[32];
    headerTester[8]='\0';
    mediaData.device()->reset();
    mediaData.readRawData(headerCache, 32);
    memcpy(headerTester, headerCache, 8);
    if(strcmp(headerTester, m_apePreamble)==0)
    {
        //Header is at the very beginning, the worst one.
        //We have to cache all the file and then write the new tag and the whole file.
        //Calculate the tag size first.
        originalTagSize=(((quint32)headerCache[15]<<24)&0b11111111000000000000000000000000)+
                        (((quint32)headerCache[14]<<16)&0b00000000111111110000000000000000)+
                        (((quint32)headerCache[13]<<8) &0b00000000000000001111111100000000)+
                        ( (quint32)headerCache[12]     &0b00000000000000000000000011111111);
        //So the real music data size is the whole file minus the tag contents and two
        //'headers' size.
        realDataSize=mediaFile.size()-originalTagSize-64;
        //Skip the tag, read all the file data.
        mediaData.skipRawData(realDataSize+32);
        char *realData=new char[realDataSize];
        mediaData.readRawData(realData, realDataSize);
        //Reset back to begin of the file.
        mediaData.device()->reset();
        //Write the tag at the beginning.
        mediaData.writeRawData(tagHeader, 32);
        mediaData.writeRawData(tagContentData.data(), tagContentData.size());
        tagHeader[23]=0b00000001;
        mediaData.writeRawData(tagHeader, 32);
        //Write the real data back to the file.
        mediaData.writeRawData(realData, realDataSize);
        //Recover the memory.
        delete[] realData;
        //Resize the file, clear the no use data.
        mediaFile.resize((quint64)(realDataSize+tagContentData.size()+64));
        return;
    }
    headerPos=mediaFile.size()-32;
    mediaData.device()->reset();
    mediaData.skipRawData(headerPos);
    mediaData.readRawData(headerCache, 32);
    memcpy(headerTester, headerCache, 8);
    if(strcmp(headerTester, m_apePreamble)==0)
    {
        //This is the best one, the frame is at the end of the file,
        //just rewrite the data!
        //If we detect this, this must be the footer of the frame.
        originalTagSize=(((quint32)headerCache[15]<<24)&0b11111111000000000000000000000000)+
                        (((quint32)headerCache[14]<<16)&0b00000000111111110000000000000000)+
                        (((quint32)headerCache[13]<<8) &0b00000000000000001111111100000000)+
                        ( (quint32)headerCache[12]     &0b00000000000000000000000011111111);
        //So the header was begin at: footer - originalTagSize - headerSize(32).
        headerPos-=(originalTagSize+32);
        //Set the the header
        mediaData.device()->reset();
        mediaData.skipRawData(headerPos);
        //Rewrite!
        mediaData.writeRawData(tagHeader, 32);
        mediaData.writeRawData(tagContentData.data(), tagContentData.size());
        tagHeader[23]=0b00000001;
        mediaData.writeRawData(tagHeader, 32);
        mediaFile.resize(headerPos+tagContentData.size()+64);
        return;
    }
    headerPos=mediaFile.size()-160;
    mediaData.device()->reset();
    mediaData.skipRawData(headerPos);
    mediaData.readRawData(headerCache, 32);
    memcpy(headerTester, headerCache, 8);
    if(strcmp(headerTester, m_apePreamble)==0)
    {
        //This is not so bad, the tag is before ID3v1. Just copy ID3v1 and rewrite.
        //Nearly the same as above.
        //Backup ID3v1 data.
        char id3v1Data[128];
        mediaData.readRawData(id3v1Data, 128);
        //If we detect this, this must be the footer of the frame.
        originalTagSize=(((quint32)headerCache[15]<<24)&0b11111111000000000000000000000000)+
                        (((quint32)headerCache[14]<<16)&0b00000000111111110000000000000000)+
                        (((quint32)headerCache[13]<<8) &0b00000000000000001111111100000000)+
                        ( (quint32)headerCache[12]     &0b00000000000000000000000011111111);
        //So the header was begin at: footer - originalTagSize - headerSize(32).
        headerPos-=(originalTagSize+32);
        //Set the the header
        mediaData.device()->reset();
        mediaData.skipRawData(headerPos);
        //Write APEv2 tag.
        mediaData.writeRawData(tagHeader, 32);
        mediaData.writeRawData(tagContentData.data(), tagContentData.size());
        tagHeader[23]=0b00000001;
        mediaData.writeRawData(tagHeader, 32);
        //Write ID3v1 data.
        mediaData.writeRawData(id3v1Data, 128);
        mediaFile.resize(headerPos+tagContentData.size()+192);
        return;
    }
    //So if we are here, it means we cannot find the APEv2 data.
    //That means, we can write anywhere, of course I'd like to write at the end.
    //So check if there's ID3v1, backup it.
    char id3Check[128];
    headerPos=mediaFile.size()-128;
    mediaData.device()->reset();
    mediaData.skipRawData(headerPos);
    mediaData.readRawData(id3Check, 128);
    if(id3Check[0]=='T' && id3Check[1]=='A' && id3Check[2]=='G')
    {
        //Find ID3v1, reset the position.
        mediaData.device()->reset();
        mediaData.skipRawData(headerPos);
        //Write APEv2
        mediaData.writeRawData(tagHeader, 32);
        mediaData.writeRawData(tagContentData.data(), tagContentData.size());
        tagHeader[23]=0b00000001;
        mediaData.writeRawData(tagHeader, 32);
        //Write ID3v1
        mediaData.writeRawData(id3Check, 128);
        return;
    }
    //No ID3v1, ok, enjoy write!
    mediaData.writeRawData(tagHeader, 32);
    mediaData.writeRawData(tagContentData.data(), tagContentData.size());
    tagHeader[23]=0b00000001;
    mediaData.writeRawData(tagHeader, 32);
}

bool KNMusicTagAPEv2::checkHeaderAndParse(QDataStream &mediaData)
{
    mediaData.device()->reset();
    mediaData.skipRawData(m_headerPosition);
    mediaData.readRawData(m_apeHeader, 32);
    m_preambleCheck[8]='\0';
    memcpy(m_preambleCheck, m_apeHeader, 8);
    if(strcmp(m_preambleCheck, m_apePreamble)==0)
    {
        m_versionNumber=(((quint32)m_apeHeader[11]<<24)&0b11111111000000000000000000000000)+
                        (((quint32)m_apeHeader[10]<<16)&0b00000000111111110000000000000000)+
                        (((quint32)m_apeHeader[9] <<8) &0b00000000000000001111111100000000)+
                        ( (quint32)m_apeHeader[8]      &0b00000000000000000000000011111111);
        m_tagSize=(((quint32)m_apeHeader[15]<<24)&0b11111111000000000000000000000000)+
                  (((quint32)m_apeHeader[14]<<16)&0b00000000111111110000000000000000)+
                  (((quint32)m_apeHeader[13]<<8) &0b00000000000000001111111100000000)+
                  ( (quint32)m_apeHeader[12]     &0b00000000000000000000000011111111);
        m_itemCount=(((quint32)m_apeHeader[19]<<24)&0b11111111000000000000000000000000)+
                    (((quint32)m_apeHeader[18]<<16)&0b00000000111111110000000000000000)+
                    (((quint32)m_apeHeader[17]<<8) &0b00000000000000001111111100000000)+
                    ( (quint32)m_apeHeader[16]     &0b00000000000000000000000011111111);
        m_tagsFlags=(((quint32)m_apeHeader[23]<<24)&0b11111111000000000000000000000000)+
                    (((quint32)m_apeHeader[22]<<16)&0b00000000111111110000000000000000)+
                    (((quint32)m_apeHeader[21]<<8) &0b00000000000000001111111100000000)+
                    ( (quint32)m_apeHeader[20]     &0b00000000000000000000000011111111);
        return true;
    }
    return false;
}

bool KNMusicTagAPEv2::readTagAt(QDataStream &mediaData)
{
    mediaData.device()->reset();
    mediaData.skipRawData(m_headerPosition);
    char *rawTagData=new char[m_tagSize];
    mediaData.readRawData(rawTagData, m_tagSize);

    //All process code here.
    quint32 currentPosition=0,
            currentFrameSize;
    int labelStart, labelEnd;
    for(quint32 i=0; i<m_itemCount; i++)
    {
        currentFrameSize=(((quint32)rawTagData[currentPosition+3]<<24)&0b11111111000000000000000000000000)+
                         (((quint32)rawTagData[currentPosition+2]<<16)&0b00000000111111110000000000000000)+
                         (((quint32)rawTagData[currentPosition+1]<<8) &0b00000000000000001111111100000000)+
                         ( (quint32)rawTagData[currentPosition]       &0b00000000000000000000000011111111);
        if(currentFrameSize>m_tagSize)
        {
            break;
        }
        labelStart=currentPosition+8;
        labelEnd=labelStart;
        while(rawTagData[labelEnd]!=0)
        {
            labelEnd++;
        }
        m_keys.append(QString(rawTagData+labelStart).toUpper());
        m_data.append(QByteArray(rawTagData+labelEnd+1, currentFrameSize));
        currentPosition=labelEnd+currentFrameSize+1;
    }
    //All process code above.

    delete[] rawTagData; //Don't touch this.
    return true;
}
