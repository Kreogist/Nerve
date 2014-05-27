#include <QComboBox>
#include <QStringList>
#include <QLineEdit>
#include <QSignalMapper>
#include <QFormLayout>
#include <QToolButton>
#include <QTextCodec>
#include <QBoxLayout>

#include <QDebug>

#include "../Libraries/knmusictagid3v1.h"

#include "../knmusicglobal.h"
#include "../../knglobal.h"

#include "knmusicid3v1editor.h"

KNMusicID3v1Editor::KNMusicID3v1Editor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    retranslate();
    m_localCodec=KNGlobal::instance()->codecForCurrentLocale();
    //ID3v1 is very simple, so use a single form layout is ok.
    m_mainLayout=new QFormLayout(this);
    m_mainLayout->setLabelAlignment(Qt::AlignRight);
    m_mainLayout->setVerticalSpacing(0);

    //Init tag reader/writer.
    m_tagID3v1=new KNMusicTagID3v1(this);

    for(int i=0; i<ID3v1ItemsCount; i++)
    {
        m_captionsLabel[i]=new QLabel(m_captions[i],
                                 this);
        //Here's a hack: to let the label fit the margins.
        m_captionsLabel[i]->setContentsMargins(0,5,0,0);
    }

    //Set the text limitations.
    m_textLimit[Title]=30;
    m_textLimit[Artist]=30;
    m_textLimit[Album]=30;
    m_textLimit[Year]=4;
    m_textLimit[Comment]=28;
    m_textLimit[Track]=3;

    //Text size counter.
    QSignalMapper *textCounter=new QSignalMapper(this);
    connect(textCounter, SIGNAL(mapped(int)),
            this, SLOT(updateCount(int)));

    for(int i=0; i<=Track; i++)
    {
        //Every text items will have an editor and a text counter, in a BoxLayout.
        QBoxLayout *editField=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_mainLayout->widget());
        //Spacing hack.
        editField->setContentsMargins(0,5,0,0);
        editField->setSpacing(0);
        //Editor widget.
        m_textEditor[i]=new QLineEdit(this);
        connect(m_textEditor[i], SIGNAL(textChanged(QString)),
                textCounter, SLOT(map()));
        textCounter->setMapping(m_textEditor[i], i);
        editField->addWidget(m_textEditor[i]);
        editField->addSpacing(10);
        //Text counter.
        m_textCountLabel[i]=new QLabel(this);
        m_textCountLabel[i]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        m_textCountLabel[i]->setMinimumWidth(fontMetrics().width("QQ"));
        editField->addWidget(m_textCountLabel[i]);
        m_textLimitLabel[i]=new QLabel("/"+QString::number(m_textLimit[i]),
                                       this);
        m_textLimitLabel[i]->setMinimumWidth(fontMetrics().width("QQQ"));
        editField->addWidget(m_textLimitLabel[i]);
        m_mainLayout->addRow(m_captionsLabel[i], editField);
        //Real set widget, when the text was larger than the limitations,
        //this widget will tell you what the real set text is.
        m_willSetAs[i]=new QLabel(this);
        m_willSetAs[i]->setContentsMargins(0,3,0,0);
        m_willSetAs[i]->setVisible(false);
        m_mainLayout->addRow("", m_willSetAs[i]);
    }
    //Because the combo box cannot set contents margins, use a layout hack.
    QBoxLayout *genreField=new QBoxLayout(QBoxLayout::LeftToRight,
                                         m_mainLayout->widget());
    genreField->setContentsMargins(0,5,0,0);
    m_genreCombo=new QComboBox(this);
    genreField->addWidget(m_genreCombo);
    QStringList genreList("-------");
    genreList.append(KNMusicGlobal::instance()->genreList());
    m_genreCombo->addItems(genreList);
    m_mainLayout->addRow(m_captionsLabel[Genre], genreField);
}

void KNMusicID3v1Editor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    //Clear all the current data.
    resetEditor();
    //Reset the file.
    mediaFile.reset();
    //Read the tag.
    if(m_tagID3v1->readTag(mediaFile, mediaData))
    {
        //Set all the data.
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

void KNMusicID3v1Editor::resetEditor()
{
    //Clear text, set genre to 0.
    for(int i=0; i<6; i++)
    {
        m_textEditor[i]->clear();
    }
    m_genreCombo->setCurrentIndex(0);
}

//Return the specific data for overview.
QString KNMusicID3v1Editor::title() const
{
    return m_tagID3v1->textData(KNMusicTagID3v1::Title);
}

QString KNMusicID3v1Editor::album() const
{
    return m_tagID3v1->textData(KNMusicTagID3v1::Album);
}

QString KNMusicID3v1Editor::artist() const
{
    return m_tagID3v1->textData(KNMusicTagID3v1::Artist);
}

void KNMusicID3v1Editor::retranslate()
{
    m_captions[Title]=tr("Title:");
    m_captions[Artist]=tr("Artist:");
    m_captions[Album]=tr("Album:");
    m_captions[Year]=tr("Year:");
    m_captions[Comment]=tr("Comment:");
    m_captions[Track]=tr("Track:");
    m_captions[Genre]=tr("Genre:");
    m_realSetText=tr("This will be set as: %1");
}

void KNMusicID3v1Editor::retranslateAndSet()
{
    ;
}

void KNMusicID3v1Editor::writeTag(QFile &mediaFile,
                                  QDataStream &mediaData)
{
    m_tagID3v1->setTextData(KNMusicTagID3v1::Title,  m_textEditor[Title]->text());
    m_tagID3v1->setTextData(KNMusicTagID3v1::Artist, m_textEditor[Artist]->text());
    m_tagID3v1->setTextData(KNMusicTagID3v1::Album,  m_textEditor[Album]->text());
    m_tagID3v1->setTextData(KNMusicTagID3v1::Year,   m_textEditor[Year]->text());
    m_tagID3v1->setTextData(KNMusicTagID3v1::Comment,m_textEditor[Comment]->text());
    m_tagID3v1->setTextData(KNMusicTagID3v1::Track,  m_textEditor[Track]->text());
    m_tagID3v1->setTextData(KNMusicTagID3v1::Genre,  m_genreCombo->currentText());
    m_tagID3v1->writeTag(mediaFile, mediaData);
}

void KNMusicID3v1Editor::updateCount(const int i)
{
    int currentTextSize=m_localCodec->fromUnicode(m_textEditor[i]->text()).size();
    QPalette labelPal=m_textCountLabel[i]->palette();
    if(currentTextSize>m_textLimit[i])
    {
        labelPal.setColor(QPalette::WindowText, QColor(255,0,0));
        m_realText[m_textLimit[i]]='\0';
        strncpy(m_realText, m_textEditor[i]->text().toStdString().data(), m_textLimit[i]);
        m_willSetAs[i]->setText(m_realSetText.arg(m_realText));
        m_willSetAs[i]->setVisible(true);
    }
    else
    {
        labelPal.setColor(QPalette::WindowText, QColor(0,0,0));
        m_willSetAs[i]->setVisible(false);
    }
    m_textCountLabel[i]->setPalette(labelPal);
    m_textCountLabel[i]->setText(QString::number(currentTextSize));
}

