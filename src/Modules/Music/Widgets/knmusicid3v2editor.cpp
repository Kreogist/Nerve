#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QStringList>
#include <QTreeView>

#include "../Libraries/knmusictagid3v2.h"

#include "../knmusicglobal.h"

#include "knmusicid3v2editor.h"

KNMusicID3v2Editor::KNMusicID3v2Editor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    retranslate();
    m_tagID3v2=new KNMusicTagID3v2(this);

    QGridLayout *mainLayout=new QGridLayout(this);
    setLayout(mainLayout);

    for(int i=0; i<ID3v2CaptionItemsCount; i++)
    {
        m_label[i]=new QLabel(m_caption[i], this);
    }
    mainLayout->addWidget(m_label[CaptionName], 0, 0, 1, 1, Qt::AlignRight);
    m_textEditor[Name]=new QLineEdit(this);
    mainLayout->addWidget(m_textEditor[Name], 0, 1, 1, 3);
    mainLayout->addWidget(m_label[CaptionArtist], 1, 0, 1, 1, Qt::AlignRight);
    m_textEditor[Artist]=new QLineEdit(this);
    mainLayout->addWidget(m_textEditor[Artist], 1, 1, 1, 1);

    mainLayout->addWidget(m_label[CaptionYear], 1, 2, 1, 1, Qt::AlignRight);
    m_textEditor[Year]=new QLineEdit(this);
    m_textEditor[Year]->setMaximumWidth(50);
    m_textEditor[Year]->setAlignment(Qt::AlignRight);
    mainLayout->addWidget(m_textEditor[Year], 1, 3, 1, 1);

    mainLayout->addWidget(m_label[CaptionAlbum], 2, 0, 1, 1, Qt::AlignRight);
    m_textEditor[Album]=new QLineEdit(this);
    mainLayout->addWidget(m_textEditor[Album], 2, 1, 1, 1);

    mainLayout->addWidget(m_label[CaptionTrack], 2, 2, 1, 1, Qt::AlignRight);
    m_textEditor[TrackNumber]=new QLineEdit(this);
    m_textEditor[TrackNumber]->setFixedWidth(35);
    m_textEditor[TrackNumber]->setAlignment(Qt::AlignRight);
    QLabel *trackSlash=new QLabel("/", this);
    m_textEditor[TrackCount]=new QLineEdit(this);
    m_textEditor[TrackCount]->setFixedWidth(35);
    m_textEditor[TrackCount]->setAlignment(Qt::AlignRight);
    QBoxLayout *trackLayout=new QBoxLayout(QBoxLayout::LeftToRight, mainLayout->widget());
    trackLayout->addWidget(m_textEditor[TrackNumber]);
    trackLayout->addWidget(trackSlash,1);
    trackLayout->addWidget(m_textEditor[TrackCount]);
    mainLayout->addLayout(trackLayout, 2, 3, 1, 1);

    mainLayout->addWidget(m_label[CaptionAlbumArtist], 3, 0, 1, 1, Qt::AlignRight);
    m_textEditor[AlbumArtist]=new QLineEdit(this);
    mainLayout->addWidget(m_textEditor[AlbumArtist], 3, 1, 1, 1);

    mainLayout->addWidget(m_label[CaptionDisc], 3, 2, 1, 1, Qt::AlignRight);
    m_textEditor[DiscNumber]=new QLineEdit(this);
    m_textEditor[DiscNumber]->setFixedWidth(35);
    m_textEditor[DiscNumber]->setAlignment(Qt::AlignRight);
    QLabel *discSlash=new QLabel("/", this);
    m_textEditor[DiscCount]=new QLineEdit(this);
    m_textEditor[DiscCount]->setFixedWidth(35);
    m_textEditor[DiscCount]->setAlignment(Qt::AlignRight);
    QBoxLayout *discLayout=new QBoxLayout(QBoxLayout::LeftToRight, mainLayout->widget());
    discLayout->addWidget(m_textEditor[DiscNumber]);
    discLayout->addWidget(discSlash,1);
    discLayout->addWidget(m_textEditor[DiscCount]);
    mainLayout->addLayout(discLayout, 3, 3, 1, 1);

    mainLayout->addWidget(m_label[CaptionGenre], 4, 0, 1, 1, Qt::AlignRight);
    m_genreList=new QComboBox(this);
    m_genreList->setEditable(true);
    mainLayout->addWidget(m_genreList, 4, 1, 1, 3);
    mainLayout->addWidget(m_label[CaptionComment], 5, 0, 1, 1, Qt::AlignRight);
    m_commentEditor=new QPlainTextEdit(this);
    mainLayout->addWidget(m_commentEditor, 5, 1, 1, 3);

    mainLayout->setColumnStretch(1, 1);

    m_advancedView=new QTreeView(this);
    m_advancedView->setIndentation(0);
    m_advancedView->resize(200,200);
}

void KNMusicID3v2Editor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    resetEditor();
    mediaFile.reset();
    m_advancedModel.reset(new QStandardItemModel);
    QStringList advancedHeader;
    advancedHeader<<tr("Frame")<<tr("Frame Data");
    m_advancedModel->setHorizontalHeaderLabels(advancedHeader);
    if(m_tagID3v2->readTag(mediaFile, mediaData))
    {
        m_textEditor[Name]->setText(m_tagID3v2->textData(KNMusicTagID3v2::Name));
        m_textEditor[Artist]->setText(m_tagID3v2->textData(KNMusicTagID3v2::Artist));
        m_textEditor[Album]->setText(m_tagID3v2->textData(KNMusicTagID3v2::Album));
        m_textEditor[AlbumArtist]->setText(m_tagID3v2->textData(KNMusicTagID3v2::AlbumArtist));
        m_textEditor[Year]->setText(m_tagID3v2->textData(KNMusicTagID3v2::Year));
        QString trackInfo=m_tagID3v2->textData(KNMusicTagID3v2::Track);
        int diagonalPos=trackInfo.indexOf("/");
        if(diagonalPos!=-1)
        {
            m_textEditor[TrackNumber]->setText(trackInfo.left(diagonalPos));
            m_textEditor[TrackCount]->setText(trackInfo.mid(diagonalPos+1));
        }
        else
        {
            m_textEditor[TrackNumber]->setText(trackInfo);
        }
        trackInfo=m_tagID3v2->textData(KNMusicTagID3v2::Disc);
        diagonalPos=trackInfo.indexOf("/");
        if(diagonalPos!=-1)
        {
            m_textEditor[DiscNumber]->setText(trackInfo.left(diagonalPos));
            m_textEditor[DiscCount]->setText(trackInfo.mid(diagonalPos+1));
        }
        else
        {
            m_textEditor[DiscNumber]->setText(trackInfo);
        }
        m_genreList->setEditText(KNMusicGlobal::instance()->getGenre
                                 (m_tagID3v2->textData(KNMusicTagID3v2::Genre)));
        m_commentEditor->setPlainText(m_tagID3v2->rawTextData(KNMusicTagID3v2::Comments));

        QStringList keyList=m_tagID3v2->keyList();
        QStandardItem *currentItem;
        for(int i=0, keyListCount=keyList.size();
            i<keyListCount;
            i++)
        {
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(keyList.at(i));
            currentRow.append(currentItem);
            currentItem=new QStandardItem(m_tagID3v2->frameTextData(keyList.at(i)));
            currentRow.append(currentItem);
            m_advancedModel->appendRow(currentRow);
        }
        m_advancedView->setModel(m_advancedModel.data());
        m_advancedView->resizeColumnToContents(0);
    }
    /*setMusicCover(m_tagID3v2->tagImage(3)); //3 is the Cover front.
    if(m_musicCover.isNull())
    {
        setMusicCover(m_tagID3v2->firstAvaliableImage());
    }*/
}

void KNMusicID3v2Editor::resetEditor()
{
    ;
}

KNMusicTagBase *KNMusicID3v2Editor::musicTagReader()
{
    return m_tagID3v2;
}

void KNMusicID3v2Editor::retranslate()
{
    m_caption[CaptionName]=tr("Title:");
    m_caption[CaptionArtist]=tr("Artist:");
    m_caption[CaptionAlbum]=tr("Album:");
    m_caption[CaptionYear]=tr("Year:");
    m_caption[CaptionAlbumArtist]=tr("Album Artist:");
    m_caption[CaptionTrack]=tr("Track:");
    m_caption[CaptionDisc]=tr("Disc:");
    m_caption[CaptionComment]=tr("Comment:");
    m_caption[CaptionGenre]=tr("Genre:");
}

void KNMusicID3v2Editor::retranslateAndSet()
{
    retranslate();
}
