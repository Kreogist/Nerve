#include <QFormLayout>

#include "knmusicapev2editor.h"

KNMusicAPEv2Editor::KNMusicAPEv2Editor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    retranslate();
    QWidget *overallMode=new QWidget(this);
    for(int i=0; i<APEv2CaptionItemsCount; i++)
    {
        m_label=new QLabel(m_caption[i], this);
    }
    ;
}

void KNMusicAPEv2Editor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    ;
}

void KNMusicAPEv2Editor::resetEditor()
{
    ;
}

KNMusicTagBase *KNMusicAPEv2Editor::musicTagReader()
{
    ;
}

QString KNMusicAPEv2Editor::title() const
{
    ;
}

QString KNMusicAPEv2Editor::album() const
{
    ;
}

QString KNMusicAPEv2Editor::artist() const
{
    ;
}

void KNMusicAPEv2Editor::retranslate()
{
    m_caption[CaptionName]=tr("Name:");
    m_caption[CaptionArtist]=tr("Artist:");
    m_caption[CaptionAlbum]=tr("Album:");
    m_caption[CaptionYear]=tr("Year:");
    m_caption[CaptionTrack]=tr("Track:");
    m_caption[CaptionGenre]=tr("Genre:");
    m_caption[CaptionComment]=tr("Comment:");
}
