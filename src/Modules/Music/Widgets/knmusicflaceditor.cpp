#include <QAction>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QBoxLayout>
#include <QPlainTextEdit>
#include <QStringList>
#include <QTreeView>
#include <QToolBar>

#include <QDebug>

#include "../Libraries/knmusictagflac.h"
#include "../../Base/knverticalwidgetswitcher.h"

#include "../knmusicglobal.h"

#include "knmusicflaceditor.h"

KNMusicFLACEditor::KNMusicFLACEditor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    //Init captions.
    retranslate();
    //Init tag reader.
    m_tagFLAC=new KNMusicTagFLAC(this);

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

    for(int i=0; i<FLACCaptionItemsCount; i++)
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

void KNMusicFLACEditor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    //Reset current editor and model, clear caches.
    resetEditor();

    //Reset the media file, it might be used by other files.
    mediaFile.reset();
    //Use tag reader to read the tag. If tag exsist, display the data.
    if(m_tagFLAC->readTag(mediaFile, mediaData))
    {
        setEditorText(Name        ,m_tagFLAC->textData(KNMusicTagFLAC::Name));
        setEditorText(Artist      ,m_tagFLAC->textData(KNMusicTagFLAC::Artist));
        setEditorText(Album       ,m_tagFLAC->textData(KNMusicTagFLAC::Album));
        setEditorText(DiscCount   ,m_tagFLAC->textData(KNMusicTagFLAC::DiscCount));
        setEditorText(DiscNumber  ,m_tagFLAC->textData(KNMusicTagFLAC::DiscNumber));
        setEditorText(AlbumArtist ,m_tagFLAC->textData(KNMusicTagFLAC::AlbumArtist));
        setEditorText(TrackCount  ,m_tagFLAC->textData(KNMusicTagFLAC::TrackCount));
        setEditorText(TrackNumber ,m_tagFLAC->textData(KNMusicTagFLAC::TrackNumber));
        setEditorText(Year        ,m_tagFLAC->textData(KNMusicTagFLAC::Year).left(4));
        m_genreList->setEditText(m_tagFLAC->textData(KNMusicTagFLAC::Genre));
        m_commentEditor->setPlainText(m_tagFLAC->textData(KNMusicTagFLAC::Comments));

        //Add all frames to advanced view.
        QStringList keyList=m_tagFLAC->keyList();
        QStandardItem *currentItem;
        for(int i=0, keyListCount=keyList.size();
            i<keyListCount;
            i++)
        {
            //Create data row
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(keyList.at(i));
            currentRow.append(currentItem);
            currentItem=new QStandardItem(m_tagFLAC->rawMetaData(keyList.at(i)));
            currentRow.append(currentItem);
            //Append to model
            m_advancedModel->appendRow(currentRow);
        }
        m_advancedView->setModel(m_advancedModel.data());
        m_advancedView->resizeColumnToContents(0);
    }
}

void KNMusicFLACEditor::resetEditor()
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

QString KNMusicFLACEditor::title() const
{
    //Return the name of the song.
    return m_tagFLAC->textData(KNMusicTagFLAC::Name);
}

QString KNMusicFLACEditor::album() const
{
    return m_tagFLAC->textData(KNMusicTagFLAC::Album);
}

QString KNMusicFLACEditor::artist() const
{
    return m_tagFLAC->textData(KNMusicTagFLAC::Artist);
}

QPixmap KNMusicFLACEditor::albumArt() const
{
    //3 is the Cover front.
    QPixmap coverArt=QPixmap::fromImage(m_tagFLAC->tagImage(3));
    return coverArt.isNull()?QPixmap::fromImage(m_tagFLAC->firstAvaliableImage()):
                             coverArt;
}

void KNMusicFLACEditor::retranslate()
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

void KNMusicFLACEditor::retranslateAndSet()
{
    retranslate();
}

void KNMusicFLACEditor::toAdvancedMode()
{
    m_switcher->setCurrentIndex(1);
}

void KNMusicFLACEditor::toOverviewMode()
{
    m_switcher->setCurrentIndex(0);
}

void KNMusicFLACEditor::setEditorText(const int &index, const QString &text)
{
    m_textEditor[index]->setText(text);
    m_textEditor[index]->setCursorPosition(0);
}
