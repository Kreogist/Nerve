#include <QAction>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QStringList>
#include <QTreeView>
#include <QToolBar>

#include "../Libraries/knmusictagid3v2.h"
#include "../../Base/knverticalwidgetswitcher.h"

#include "../knmusicglobal.h"

#include "knmusicid3v2editor.h"

KNMusicID3v2Editor::KNMusicID3v2Editor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    //Init captions.
    retranslate();
    //Init tag reader.
    m_tagID3v2=new KNMusicTagID3v2(this);

    //The main layout of the editor, it will contains a toolbar and a switcher.
    QBoxLayout *editorLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    editorLayout->setSpacing(0);
    setLayout(editorLayout);

    /*The toolbar, it contains several tools for text edit and overview/advanced
      mode switcher.*/
    QToolBar *toolBar=new QToolBar(this);
    editorLayout->addWidget(toolBar);

    //The mode switcher
    m_switcher=new KNVerticalWidgetSwitcher(this);
    editorLayout->addWidget(m_switcher);

    //Overall mode container widget. You can edit some data much quickly.
    //P.S.: The layout is a little strange, don't mind these details.
    QWidget *overallMode=new QWidget(this);
    overallMode->setContentsMargins(0,0,0,0);
    QGridLayout *overallLayout=new QGridLayout(overallMode);
    overallMode->setLayout(overallLayout);

    for(int i=0; i<ID3v2CaptionItemsCount; i++)
    {
        m_label[i]=new QLabel(m_caption[i], this);
    }
    overallLayout->addWidget(m_label[CaptionName], 0, 0, 1, 1, Qt::AlignRight);
    m_textEditor[Name]=new QLineEdit(this);
    overallLayout->addWidget(m_textEditor[Name], 0, 1, 1, 3);
    overallLayout->addWidget(m_label[CaptionArtist], 1, 0, 1, 1, Qt::AlignRight);
    m_textEditor[Artist]=new QLineEdit(this);
    overallLayout->addWidget(m_textEditor[Artist], 1, 1, 1, 1);

    overallLayout->addWidget(m_label[CaptionYear], 1, 2, 1, 1, Qt::AlignRight);
    m_textEditor[Year]=new QLineEdit(this);
    m_textEditor[Year]->setMaximumWidth(50);
    m_textEditor[Year]->setAlignment(Qt::AlignRight);
    overallLayout->addWidget(m_textEditor[Year], 1, 3, 1, 1);

    overallLayout->addWidget(m_label[CaptionAlbum], 2, 0, 1, 1, Qt::AlignRight);
    m_textEditor[Album]=new QLineEdit(this);
    overallLayout->addWidget(m_textEditor[Album], 2, 1, 1, 1);

    overallLayout->addWidget(m_label[CaptionTrack], 2, 2, 1, 1, Qt::AlignRight);
    m_textEditor[TrackNumber]=new QLineEdit(this);
    m_textEditor[TrackNumber]->setFixedWidth(35);
    m_textEditor[TrackNumber]->setAlignment(Qt::AlignRight);
    QLabel *trackSlash=new QLabel("/", this);
    m_textEditor[TrackCount]=new QLineEdit(this);
    m_textEditor[TrackCount]->setFixedWidth(35);
    m_textEditor[TrackCount]->setAlignment(Qt::AlignRight);
    QBoxLayout *trackLayout=new QBoxLayout(QBoxLayout::LeftToRight, overallLayout->widget());
    trackLayout->addWidget(m_textEditor[TrackNumber]);
    trackLayout->addWidget(trackSlash,1);
    trackLayout->addWidget(m_textEditor[TrackCount]);
    overallLayout->addLayout(trackLayout, 2, 3, 1, 1);

    overallLayout->addWidget(m_label[CaptionAlbumArtist], 3, 0, 1, 1, Qt::AlignRight);
    m_textEditor[AlbumArtist]=new QLineEdit(this);
    overallLayout->addWidget(m_textEditor[AlbumArtist], 3, 1, 1, 1);

    overallLayout->addWidget(m_label[CaptionDisc], 3, 2, 1, 1, Qt::AlignRight);
    m_textEditor[DiscNumber]=new QLineEdit(this);
    m_textEditor[DiscNumber]->setFixedWidth(35);
    m_textEditor[DiscNumber]->setAlignment(Qt::AlignRight);
    QLabel *discSlash=new QLabel("/", this);
    m_textEditor[DiscCount]=new QLineEdit(this);
    m_textEditor[DiscCount]->setFixedWidth(35);
    m_textEditor[DiscCount]->setAlignment(Qt::AlignRight);
    QBoxLayout *discLayout=new QBoxLayout(QBoxLayout::LeftToRight, overallLayout->widget());
    discLayout->addWidget(m_textEditor[DiscNumber]);
    discLayout->addWidget(discSlash,1);
    discLayout->addWidget(m_textEditor[DiscCount]);
    overallLayout->addLayout(discLayout, 3, 3, 1, 1);

    overallLayout->addWidget(m_label[CaptionGenre], 4, 0, 1, 1, Qt::AlignRight);
    m_genreList=new QComboBox(this);
    m_genreList->setEditable(true);
    overallLayout->addWidget(m_genreList, 4, 1, 1, 3);
    overallLayout->addWidget(m_label[CaptionComment], 5, 0, 1, 1, Qt::AlignRight);
    m_commentEditor=new QPlainTextEdit(this);
    overallLayout->addWidget(m_commentEditor, 5, 1, 1, 3);

    overallLayout->setColumnStretch(1, 1);

    m_switcher->addWidget(overallMode);

    //Advanced mode container widget.
    //In advanced mode, you can see all the frames of the ID3v2.
    QWidget *advancedMode=new QWidget(this);
    QBoxLayout *advancedLayout=new QBoxLayout(QBoxLayout::TopToBottom, advancedMode);
    advancedMode->setLayout(advancedLayout);

    m_advancedView=new QTreeView(this);
    m_advancedView->setIndentation(0);
    advancedLayout->addWidget(m_advancedView);
    m_switcher->addWidget(advancedMode);

    //Toolbar actions.
    //Mode switcher.
    m_toAdvanced=new QAction(QIcon(":/Music/Resources/Music/tags/ID3v2toAdvanced.png"),
                             QString("Advanced Mode"),
                             this);
    connect(m_toAdvanced, SIGNAL(triggered()),
            this, SLOT(toAdvancedMode()));
    toolBar->addAction(m_toAdvanced);
    m_toOverview=new QAction(QIcon(":/Music/Resources/Music/tags/ID3v2toOverview.png"),
                             QString("Overview Mode"),
                             this);
    connect(m_toOverview, SIGNAL(triggered()),
            this, SLOT(toOverviewMode()));
    toolBar->addAction(m_toOverview);

    //Initial the editor. Set the editor to the default mode.
    m_switcher->setCurrentIndex(0);
}

void KNMusicID3v2Editor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    //Reset current editor and model, clear caches.
    resetEditor();

    //Reset the media file, it might be used by other files.
    mediaFile.reset();
    //Use tag reader to read the tag. If tag exsist, display the data.
    if(m_tagID3v2->readTag(mediaFile, mediaData))
    {
        setEditorText(Name, m_tagID3v2->textData(KNMusicTagID3v2::Name));
        setEditorText(Artist, m_tagID3v2->textData(KNMusicTagID3v2::Artist));
        setEditorText(Album, m_tagID3v2->textData(KNMusicTagID3v2::Album));
        setEditorText(AlbumArtist, m_tagID3v2->textData(KNMusicTagID3v2::AlbumArtist));
        setEditorText(Year, m_tagID3v2->textData(KNMusicTagID3v2::Year));
        QString trackInfo=m_tagID3v2->textData(KNMusicTagID3v2::Track);
        int diagonalPos=trackInfo.indexOf("/");
        if(diagonalPos!=-1)
        {
            setEditorText(TrackNumber, trackInfo.left(diagonalPos));
            setEditorText(TrackCount, trackInfo.mid(diagonalPos+1));
        }
        else
        {
            setEditorText(TrackNumber, trackInfo);
        }
        trackInfo=m_tagID3v2->textData(KNMusicTagID3v2::Disc);
        diagonalPos=trackInfo.indexOf("/");
        if(diagonalPos!=-1)
        {
            setEditorText(DiscNumber, trackInfo.left(diagonalPos));
            setEditorText(DiscCount, trackInfo.mid(diagonalPos+1));
        }
        else
        {
            setEditorText(DiscNumber, trackInfo);
        }
        m_genreList->setEditText(KNMusicGlobal::instance()->getGenre
                                 (m_tagID3v2->textData(KNMusicTagID3v2::Genre)));
        m_commentEditor->setPlainText(m_tagID3v2->rawTextData(KNMusicTagID3v2::Comments));

        //Add all frames to advanced view.
        QStringList keyList=m_tagID3v2->keyList();
        QStandardItem *currentItem;
        for(int i=0, keyListCount=keyList.size();
            i<keyListCount;
            i++)
        {
            //Create data row
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(keyList.at(i));
            currentRow.append(currentItem);
            currentItem=new QStandardItem(m_tagID3v2->frameTextData(keyList.at(i)));
            currentRow.append(currentItem);
            //Append to model
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
    //Clear all the line text data.
    for(int i=0; i<9; i++)
    {
        m_textEditor[i]->setText("");
    }
    //Clear the comment editor.
    m_commentEditor->clear();
    //Reset the model and add header.
    m_advancedModel.reset(new QStandardItemModel);
    QStringList advancedHeader;
    advancedHeader<<tr("Frame")<<tr("Frame Data");
    m_advancedModel->setHorizontalHeaderLabels(advancedHeader);
}

QString KNMusicID3v2Editor::title() const
{
    //Return the name of the song.
    return m_tagID3v2->textData(KNMusicTagID3v2::Name);
}

QString KNMusicID3v2Editor::album() const
{
    return m_tagID3v2->textData(KNMusicTagID3v2::Album);
}

QString KNMusicID3v2Editor::artist() const
{
    return m_tagID3v2->textData(KNMusicTagID3v2::Artist);
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

void KNMusicID3v2Editor::toAdvancedMode()
{
    m_switcher->setCurrentIndex(1);
}

void KNMusicID3v2Editor::toOverviewMode()
{
    m_switcher->setCurrentIndex(0);
}

void KNMusicID3v2Editor::setEditorText(const int &index, const QString &text)
{
    m_textEditor[index]->setText(text);
    m_textEditor[index]->setCursorPosition(0);
}
