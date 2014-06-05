#include <QBoxLayout>
#include <QTabWidget>
#include <QStandardPaths>
#include <QPushButton>

#include "knmusictageditorbase.h"
#include "knmusicid3v1editor.h"
#include "knmusicid3v2editor.h"
#include "knmusicapev2editor.h"
#include "knmusicwmaeditor.h"
#include "knmusicm4aeditor.h"
#include "knmusicflaceditor.h"
#include "knmusicwaveditor.h"

#include "knmusictageditor.h"

KNMusicTagEditor::KNMusicTagEditor(QWidget *parent) :
    QWidget(parent)
{
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    QPushButton *ok=new QPushButton("Write", this);
    connect(ok, SIGNAL(clicked()),
            this, SLOT(writeTag()));
    mainLayout->addWidget(ok);

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
    m_FLACEditor=new KNMusicFLACEditor(this);
    tabWidget->addTab(m_FLACEditor, "FLAC");
    m_WAVEditor=new KNMusicWAVEditor(this);
    tabWidget->addTab(m_WAVEditor, "WAV");
}

void KNMusicTagEditor::parseFile(const QString &filePath)
{
    resetEditor();
    m_filePath=filePath;
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
        m_FLACEditor->readTag(mediaFile, mediaData);
        readBasicInfoFromEditor(m_FLACEditor);
        m_WAVEditor->readTag(mediaFile, mediaData);
        readBasicInfoFromEditor(m_WAVEditor);
        mediaFile.close();
    }
}

void KNMusicTagEditor::writeTag()
{
    QFile mediaFile(m_filePath);
    QDataStream mediaData(&mediaFile);
    if(mediaFile.open(QIODevice::ReadWrite))
    {
        m_ID3v1Editor->writeTag(mediaFile, mediaData);
        m_APEv2Editor->writeTag(mediaFile, mediaData);
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
