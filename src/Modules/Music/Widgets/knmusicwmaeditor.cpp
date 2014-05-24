#include <QLineEdit>
#include <QToolBar>
#include <QTreeView>
#include <QAction>
#include <QFormLayout>

#include "../Libraries/knmusictagwma.h"

#include "../../Base/knverticalwidgetswitcher.h"

#include "knmusicwmaeditor.h"

KNMusicWMAEditor::KNMusicWMAEditor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    //Init captions.
    retranslate();
    //Init tag reader.
    m_tagWMA=new KNMusicTagWMA(this);

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
    //P.S.: This layout is much easier than ID3v2, great~!
    QWidget *overallMode=new QWidget(this);
    overallMode->setContentsMargins(0,0,0,0);
    QFormLayout *overallLayout=new QFormLayout(overallMode);
    overallMode->setLayout(overallLayout);

    for(int i=0; i<WMACaptionItemsCount; i++)
    {
        m_label[i]=new QLabel(m_caption[i], this);
        m_textEditor[i]=new QLineEdit(this);
        overallLayout->addRow(m_label[i], m_textEditor[i]);
    }

    m_switcher->addWidget(overallMode);

    //Advanced mode container widget.
    //In advanced mode, you can see all the frames of the WMA.
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


void KNMusicWMAEditor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    //Reset current editor and model, clear caches.
    resetEditor();
    m_advancedModel.reset(new QStandardItemModel);
    QStringList advancedHeader;
    advancedHeader<<tr("Frame")<<tr("Frame Data");
    m_advancedModel->setHorizontalHeaderLabels(advancedHeader);

    //Reset the media file, it might be used by other files.
    mediaFile.reset();
    //Use tag reader to read the tag. If tag exsist, display the data.
    if(m_tagWMA->readTag(mediaFile, mediaData))
    {
        setEditorText(Name        ,m_tagWMA->textData(KNMusicTagWMA::Name));
        setEditorText(Artist      ,m_tagWMA->textData(KNMusicTagWMA::Artist));
        setEditorText(Album       ,m_tagWMA->textData(KNMusicTagWMA::Album));
        setEditorText(Year        ,m_tagWMA->textData(KNMusicTagWMA::Year));
        setEditorText(Track       ,m_tagWMA->textData(KNMusicTagWMA::TrackNumber));
        setEditorText(Genre       ,m_tagWMA->textData(KNMusicTagWMA::Genre));
        setEditorText(Comment     ,m_tagWMA->textData(KNMusicTagWMA::Comments));

        //Add all frames to advanced view.
        QStringList keyList=m_tagWMA->keyList();
        QStandardItem *currentItem;
        for(int i=0, keyListCount=keyList.size();
            i<keyListCount;
            i++)
        {
            //Create data row
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(keyList.at(i));
            currentRow.append(currentItem);
            currentItem=new QStandardItem(m_tagWMA->frameData(keyList.at(i)));
            currentRow.append(currentItem);
            //Append to model
            m_advancedModel->appendRow(currentRow);
        }
        m_advancedView->setModel(m_advancedModel.data());
        m_advancedView->resizeColumnToContents(0);
    }
}

void KNMusicWMAEditor::resetEditor()
{
    //Reset the line text editor.
    for(int i=0; i<WMACaptionItemsCount; i++)
    {
        m_textEditor[i]->setText("");
    }
}

QString KNMusicWMAEditor::title() const
{
    //Return the name of the song.
    return m_tagWMA->textData(KNMusicTagWMA::Name);
}

QString KNMusicWMAEditor::album() const
{
    //Return the album of the song.
    return m_tagWMA->textData(KNMusicTagWMA::Album);
}

QString KNMusicWMAEditor::artist() const
{
    //Return the artist of the song.
    return m_tagWMA->textData(KNMusicTagWMA::Artist);
}

QPixmap KNMusicWMAEditor::albumArt() const
{
    //Return the album art.
    return QPixmap::fromImage(m_tagWMA->albumArt());
}

void KNMusicWMAEditor::retranslate()
{
    m_caption[Name]=tr("Name:");
    m_caption[Artist]=tr("Artist:");
    m_caption[Album]=tr("Album:");
    m_caption[Year]=tr("Year:");
    m_caption[Track]=tr("Track:");
    m_caption[Genre]=tr("Genre:");
    m_caption[Comment]=tr("Comment:");
}

void KNMusicWMAEditor::retranslateAndSet()
{
    ;
}

void KNMusicWMAEditor::toAdvancedMode()
{
    m_switcher->setCurrentIndex(1);
}

void KNMusicWMAEditor::toOverviewMode()
{
    m_switcher->setCurrentIndex(0);
}

void KNMusicWMAEditor::setEditorText(const int &index, const QString &data)
{
    m_textEditor[index]->setText(data);
    m_textEditor[index]->setCursorPosition(0);
}
