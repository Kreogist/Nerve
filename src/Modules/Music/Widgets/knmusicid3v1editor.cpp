#include <QComboBox>
#include <QStringList>
#include <QLineEdit>
#include <QFormLayout>

#include "../Libraries/knmusictagid3v1.h"

#include "../knmusicglobal.h"

#include "knmusicid3v1editor.h"

KNMusicID3v1Editor::KNMusicID3v1Editor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    retranslate();
    m_mainLayout=new QFormLayout(this);
    m_mainLayout->setLabelAlignment(Qt::AlignRight);

    m_tagID3v1=new KNMusicTagID3v1(this);

    for(int i=0; i<ID3v1ItemsCount; i++)
    {
        m_captions[i]=new QLabel(m_captionsTitle[i],
                                 this);
    }

    m_textEditor[Title]=new QLineEdit(this);
    m_textEditor[Title]->setMaxLength(30);
    m_mainLayout->addRow(m_captions[Title], m_textEditor[Title]);
    m_textEditor[Artist]=new QLineEdit(this);
    m_textEditor[Artist]->setMaxLength(30);
    m_mainLayout->addRow(m_captions[Artist], m_textEditor[Artist]);
    m_textEditor[Album]=new QLineEdit(this);
    m_textEditor[Album]->setMaxLength(30);
    m_mainLayout->addRow(m_captions[Album], m_textEditor[Album]);
    m_textEditor[Year]=new QLineEdit(this);
    m_textEditor[Year]->setMaxLength(4);
    m_mainLayout->addRow(m_captions[Year], m_textEditor[Year]);
    m_textEditor[Comment]=new QLineEdit(this);
    m_textEditor[Comment]->setMaxLength(28);
    m_mainLayout->addRow(m_captions[Comment], m_textEditor[Comment]);
    m_textEditor[Track]=new QLineEdit(this);
    m_textEditor[Track]->setMaxLength(3);
    m_mainLayout->addRow(m_captions[Track], m_textEditor[Track]);
    m_genreCombo=new QComboBox(this);
    QStringList genreList("-");
    genreList.append(KNMusicGlobal::instance()->genreList());
    m_genreCombo->addItems(genreList);
    m_mainLayout->addRow(m_captions[Genre], m_genreCombo);
}

void KNMusicID3v1Editor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    mediaFile.reset();
    if(m_tagID3v1->readTag(mediaFile, mediaData))
    {
        m_textEditor[Title]->setText(m_tagID3v1->textData(KNMusicTagID3v1::Title));
        m_textEditor[Artist]->setText(m_tagID3v1->textData(KNMusicTagID3v1::Artist));
        m_textEditor[Album]->setText(m_tagID3v1->textData(KNMusicTagID3v1::Album));
        m_textEditor[Year]->setText(m_tagID3v1->textData(KNMusicTagID3v1::Year));
        m_textEditor[Comment]->setText(m_tagID3v1->textData(KNMusicTagID3v1::Comment));
        m_textEditor[Track]->setText(m_tagID3v1->textData(KNMusicTagID3v1::Track));
        quint8 genreIndex=m_tagID3v1->genreIndex();
        if(genreIndex<KNMusicGlobal::instance()->genreListSize())
        {
            m_genreCombo->setCurrentIndex(genreIndex+1);
        }
        else
        {
            m_genreCombo->setCurrentIndex(0);
        }
    }
}

void KNMusicID3v1Editor::retranslate()
{
    m_captionsTitle[Title]=tr("Title:");
    m_captionsTitle[Artist]=tr("Artist:");
    m_captionsTitle[Album]=tr("Album:");
    m_captionsTitle[Year]=tr("Year:");
    m_captionsTitle[Comment]=tr("Comment:");
    m_captionsTitle[Track]=tr("Track:");
    m_captionsTitle[Genre]=tr("Genre:");
}

void KNMusicID3v1Editor::retranslateAndSet()
{
    ;
}

