#include <QBoxLayout>
#include <QTabWidget>
#include <QStandardPaths>

#include "../Libraries/knmusictagflac.h"
#include "../Libraries/knmusictagwav.h"

#include "knmusictageditorbase.h"
#include "knmusicid3v1editor.h"
#include "knmusicid3v2editor.h"
#include "knmusicapev2editor.h"
#include "knmusicwmaeditor.h"
#include "knmusicm4aeditor.h"

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
    m_APEv2Editor=new KNMusicAPEv2Editor(this);
    tabWidget->addTab(m_APEv2Editor, "APEv2");
    m_WMAEditor=new KNMusicWMAEditor(this);
    tabWidget->addTab(m_WMAEditor, "WMA");
    m_M4AEditor=new KNMusicM4AEditor(this);
    tabWidget->addTab(m_M4AEditor, "M4A");
    m_tagFLAC=new KNMusicTagFLAC(this);
    m_tagWAV=new KNMusicTagWAV(this);
}

void KNMusicTagEditor::parseFile(const QString &filePath)
{
    resetEditor();
    QFile mediaFile(filePath);
    QDataStream mediaData(&mediaFile);
    if(mediaFile.open(QIODevice::ReadOnly))
    {
        m_ID3v1Editor->readTag(mediaFile, mediaData);
        readBasicInfoFromEditor(m_ID3v1Editor);
        m_ID3v2Editor->readTag(mediaFile, mediaData);
        readBasicInfoFromEditor(m_ID3v2Editor);
        m_APEv2Editor->readTag(mediaFile, mediaData);
        readBasicInfoFromEditor(m_APEv2Editor);
        m_WMAEditor->readTag(mediaFile, mediaData);
        readBasicInfoFromEditor(m_WMAEditor);
        m_M4AEditor->readTag(mediaFile, mediaData);
        readBasicInfoFromEditor(m_M4AEditor);
        readFLACTag(mediaFile, mediaData);
        readWAVTag(mediaFile, mediaData);
        mediaFile.close();
    }
}

QString KNMusicTagEditor::title() const
{
    return m_basicInfo[Title];
}

QString KNMusicTagEditor::album() const
{
    return m_basicInfo[Album];
}

QString KNMusicTagEditor::artist() const
{
    return m_basicInfo[Artist];
}

QPixmap KNMusicTagEditor::albumArt() const
{
    return m_coverArt;
}

void KNMusicTagEditor::resetEditor()
{
    m_coverArt=QPixmap();
}

void KNMusicTagEditor::readBasicInfoFromEditor(KNMusicTagEditorBase *editor)
{
    QString cache=editor->title();
    if(!cache.isEmpty())
    {
        m_basicInfo[Title]=cache;
    }
    cache=editor->artist();
    if(!cache.isEmpty())
    {
        m_basicInfo[Artist]=cache;
    }
    cache=editor->album();
    if(!cache.isEmpty())
    {
        m_basicInfo[Album]=cache;
    }
    QPixmap coverCache=editor->albumArt();
    if(!coverCache.isNull())
    {
        m_coverArt=coverCache;
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
