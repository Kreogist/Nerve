#include <QBoxLayout>
#include <QTabWidget>

#include "../Libraries/knmusictagapev2.h"
#include "../Libraries/knmusictagwma.h"
#include "../Libraries/knmusictagm4a.h"
#include "../Libraries/knmusictagflac.h"
#include "../Libraries/knmusictagwav.h"

#include "knmusicid3v1editor.h"
#include "knmusicid3v2editor.h"

#include "knmusictageditor.h"

KNMusicTagEditor::KNMusicTagEditor(QWidget *parent) :
    QWidget(parent)
{
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    QTabWidget *tabWidget=new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    m_ID3v1Editor=new KNMusicID3v1Editor(this);
    tabWidget->addTab(m_ID3v1Editor, "ID3v1");
    m_ID3v2Editor=new KNMusicID3v2Editor(this);
    tabWidget->addTab(m_ID3v2Editor, "ID3v2");
    m_tagAPEv2=new KNMusicTagAPEv2(this);
    m_tagWMA=new KNMusicTagWMA(this);
    m_tagM4A=new KNMusicTagM4A(this);
    m_tagFLAC=new KNMusicTagFLAC(this);
    m_tagWAV=new KNMusicTagWAV(this);
}

void KNMusicTagEditor::parseFile(const QString &filePath)
{
    QFile mediaFile(filePath);
    QDataStream mediaData(&mediaFile);
    if(mediaFile.open(QIODevice::ReadOnly))
    {
        m_ID3v1Editor->readTag(mediaFile, mediaData);
        m_ID3v2Editor->readTag(mediaFile, mediaData);
        readAPEv2Tag(mediaFile, mediaData);
        readWMATag(mediaFile, mediaData);
        readM4ATag(mediaFile, mediaData);
        readFLACTag(mediaFile, mediaData);
        readWAVTag(mediaFile, mediaData);
        mediaFile.close();
    }
}

void KNMusicTagEditor::readAPEv2Tag(QFile &mediaFile,
                                        QDataStream &mediaData)
{
    mediaFile.reset();
    if(m_tagAPEv2->readTag(mediaFile,
                           mediaData))
    {
        /*setMediaData(KNMusicGlobal::Name        ,m_tagAPEv2->textData(KNMusicTagAPEv2::Name));
        setMediaData(KNMusicGlobal::Artist      ,m_tagAPEv2->textData(KNMusicTagAPEv2::Artist));
        setMediaData(KNMusicGlobal::Album       ,m_tagAPEv2->textData(KNMusicTagAPEv2::Album));
        setMediaData(KNMusicGlobal::Comments    ,m_tagAPEv2->textData(KNMusicTagAPEv2::Comments));
        setMediaData(KNMusicGlobal::Composer    ,m_tagAPEv2->textData(KNMusicTagAPEv2::Composer));
        setMediaData(KNMusicGlobal::Genre       ,m_tagAPEv2->textData(KNMusicTagAPEv2::Genre));
        setMediaData(KNMusicGlobal::Year        ,m_tagAPEv2->textData(KNMusicTagAPEv2::Year));
        setMediaData(KNMusicGlobal::TrackNumber ,m_tagAPEv2->textData(KNMusicTagAPEv2::Track));*/
    }
}

void KNMusicTagEditor::readWMATag(QFile &mediaFile,
                                      QDataStream &mediaData)
{
    mediaFile.reset();
    if(m_tagWMA->readTag(mediaFile,
                         mediaData))
    {
        /*setMediaData(KNMusicGlobal::Name           ,m_tagWMA->textData(KNMusicTagWma::Name));
        setMediaData(KNMusicGlobal::Artist         ,m_tagWMA->textData(KNMusicTagWma::Artist));
        setMediaData(KNMusicGlobal::AlbumArtist    ,m_tagWMA->textData(KNMusicTagWma::AlbumArtist));
        setMediaData(KNMusicGlobal::Album          ,m_tagWMA->textData(KNMusicTagWma::Album));
        setMediaData(KNMusicGlobal::BeatsPerMinuate,m_tagWMA->textData(KNMusicTagWma::BeatsPerMinuate));
        setMediaData(KNMusicGlobal::Comments       ,m_tagWMA->textData(KNMusicTagWma::Comments));
        setMediaData(KNMusicGlobal::Composer       ,m_tagWMA->textData(KNMusicTagWma::Composer));
        setMediaData(KNMusicGlobal::Description    ,m_tagWMA->textData(KNMusicTagWma::Description));
        setMediaData(KNMusicGlobal::Genre          ,m_tagWMA->textData(KNMusicTagWma::Genre));
        setMediaData(KNMusicGlobal::Year           ,m_tagWMA->textData(KNMusicTagWma::Year));
        setMediaData(KNMusicGlobal::TrackNumber    ,m_tagWMA->textData(KNMusicTagWma::TrackNumber));
        setMusicCover(m_tagWMA->albumArt());*/
    }
}

void KNMusicTagEditor::readM4ATag(QFile &mediaFile,
                                      QDataStream &mediaData)
{
    mediaFile.reset();
    if(m_tagM4A->readTag(mediaFile,
                         mediaData))
    {
        /*setMediaData(KNMusicGlobal::Name,           m_tagM4A->textData(KNMusicTagM4A::Title));
        setMediaData(KNMusicGlobal::Artist,         m_tagM4A->textData(KNMusicTagM4A::Artist));
        setMediaData(KNMusicGlobal::Album,          m_tagM4A->textData(KNMusicTagM4A::Album));
        setMediaData(KNMusicGlobal::AlbumArtist,    m_tagM4A->textData(KNMusicTagM4A::AlbumArtist));
        setMediaData(KNMusicGlobal::BeatsPerMinuate,m_tagM4A->textData(KNMusicTagM4A::BPM));
        setMediaData(KNMusicGlobal::Category,       m_tagM4A->textData(KNMusicTagM4A::Category));
        setMediaData(KNMusicGlobal::Composer,       m_tagM4A->textData(KNMusicTagM4A::Composer));
        setMediaData(KNMusicGlobal::Comments,       m_tagM4A->textData(KNMusicTagM4A::Comment));
        setMediaData(KNMusicGlobal::Description,    m_tagM4A->textData(KNMusicTagM4A::Description));
        setMediaData(KNMusicGlobal::Genre,          m_tagM4A->textData(KNMusicTagM4A::Genre));
        setMediaData(KNMusicGlobal::Grouping,       m_tagM4A->textData(KNMusicTagM4A::Grouping));
        setMediaData(KNMusicGlobal::Year,           m_tagM4A->textData(KNMusicTagM4A::Year));
        QByteArray trackData=m_tagM4A->metaData(KNMusicTagM4A::Tracknumber);
        if(trackData.size()>6)
        {
            setMediaData(KNMusicGlobal::TrackNumber, QString::number(trackData.at(3)));
            setMediaData(KNMusicGlobal::TrackCount, QString::number(trackData.at(5)));
        }
        setMusicCover(m_tagM4A->albumArt());*/
    }
}

void KNMusicTagEditor::readFLACTag(QFile &mediaFile,
                                       QDataStream &mediaData)
{
    mediaFile.reset();
    if(m_tagFLAC->readTag(mediaFile,
                          mediaData))
    {
        /*setMediaData(KNMusicGlobal::Name        ,m_tagFLAC->textData(KNMusicTagFLAC::Name));
        setMediaData(KNMusicGlobal::Artist      ,m_tagFLAC->textData(KNMusicTagFLAC::Artist));
        setMediaData(KNMusicGlobal::Album       ,m_tagFLAC->textData(KNMusicTagFLAC::Album));
        setMediaData(KNMusicGlobal::Genre       ,m_tagFLAC->textData(KNMusicTagFLAC::Genre));
        setMediaData(KNMusicGlobal::Description ,m_tagFLAC->textData(KNMusicTagFLAC::Description));
        setMediaData(KNMusicGlobal::Composer    ,m_tagFLAC->textData(KNMusicTagFLAC::Composer));
        setMediaData(KNMusicGlobal::DiscCount   ,m_tagFLAC->textData(KNMusicTagFLAC::DiscCount));
        setMediaData(KNMusicGlobal::DiscNumber  ,m_tagFLAC->textData(KNMusicTagFLAC::DiscNumber));
        setMediaData(KNMusicGlobal::Comments    ,m_tagFLAC->textData(KNMusicTagFLAC::Comments));
        setMediaData(KNMusicGlobal::AlbumArtist ,m_tagFLAC->textData(KNMusicTagFLAC::AlbumArtist));
        setMediaData(KNMusicGlobal::TrackCount  ,m_tagFLAC->textData(KNMusicTagFLAC::TrackCount));
        setMediaData(KNMusicGlobal::Year        ,m_tagFLAC->textData(KNMusicTagFLAC::Year).left(4));
        setMediaData(KNMusicGlobal::TrackNumber ,m_tagFLAC->textData(KNMusicTagFLAC::TrackNumber));
        setMusicCover(m_tagFLAC->tagImage(3));
        if(m_musicCover.isNull())
        {
            setMusicCover(m_tagFLAC->firstAvaliableImage());
        }*/
    }
}

void KNMusicTagEditor::readWAVTag(QFile &mediaFile, QDataStream &mediaData)
{
    mediaFile.reset();
    if(m_tagWAV->readTag(mediaFile,
                         mediaData))
    {
        /*setMediaData(KNMusicGlobal::Name            ,m_tagWAV->textData(KNMusicTagWAV::Name));
        setMediaData(KNMusicGlobal::Artist          ,m_tagWAV->textData(KNMusicTagWAV::Artist));
        setMediaData(KNMusicGlobal::Album           ,m_tagWAV->textData(KNMusicTagWAV::Album));
        setMediaData(KNMusicGlobal::AlbumArtist     ,m_tagWAV->textData(KNMusicTagWAV::AlbumArtist));
        setMediaData(KNMusicGlobal::BeatsPerMinuate ,m_tagWAV->textData(KNMusicTagWAV::BeatsPerMinuate));
        setMediaData(KNMusicGlobal::Category        ,m_tagWAV->textData(KNMusicTagWAV::Category));
        setMediaData(KNMusicGlobal::Comments        ,m_tagWAV->textData(KNMusicTagWAV::Comments));
        setMediaData(KNMusicGlobal::Composer        ,m_tagWAV->textData(KNMusicTagWAV::Composer));
        setMediaData(KNMusicGlobal::Description     ,m_tagWAV->textData(KNMusicTagWAV::Description));
        setMediaData(KNMusicGlobal::Genre           ,m_tagWAV->textData(KNMusicTagWAV::Genre));
        setMediaData(KNMusicGlobal::Year            ,m_tagWAV->textData(KNMusicTagWAV::Year));*/
    }
}
