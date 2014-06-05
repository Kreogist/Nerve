#include <QLineEdit>
#include <QToolBar>
#include <QTreeView>
#include <QAction>
#include <QSignalMapper>
#include <QFormLayout>

#include <QDebug>

#include "../Libraries/knmusictagapev2.h"

#include "../../Base/knverticalwidgetswitcher.h"

#include "knmusicapev2editor.h"

KNMusicAPEv2Editor::KNMusicAPEv2Editor(QWidget *parent) :
    KNMusicTagEditorBase(parent)
{
    //Init captions.
    retranslate();
    //Init tag reader.
    m_tagAPEv2=new KNMusicTagAPEv2(this);

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

    //Line edit finished signal mapper, will tell the model to update data.
    m_editFinish=new QSignalMapper(this);
    connect(m_editFinish, SIGNAL(mapped(int)),
            this, SLOT(onActionLineEditFinished(int)));

    for(int i=0; i<APEv2CaptionItemsCount; i++)
    {
        m_label[i]=new QLabel(m_caption[i], this);
        m_textEditor[i]=new QLineEdit(this);
        connect(m_textEditor[i], SIGNAL(editingFinished()),
                m_editFinish, SLOT(map()));
        overallLayout->addRow(m_label[i], m_textEditor[i]);
    }
    m_editFinish->setMapping(m_textEditor[Name   ], KNMusicTagAPEv2::Name);
    m_editFinish->setMapping(m_textEditor[Artist ], KNMusicTagAPEv2::Artist);
    m_editFinish->setMapping(m_textEditor[Album  ], KNMusicTagAPEv2::Album);
    m_editFinish->setMapping(m_textEditor[Year   ], KNMusicTagAPEv2::Year);
    m_editFinish->setMapping(m_textEditor[Track  ], KNMusicTagAPEv2::Track);
    m_editFinish->setMapping(m_textEditor[Genre  ], KNMusicTagAPEv2::Genre);
    m_editFinish->setMapping(m_textEditor[Comment], KNMusicTagAPEv2::Comments);

    m_switcher->addWidget(overallMode);

    //Advanced mode container widget.
    //In advanced mode, you can see all the frames of the APEv2.
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

void KNMusicAPEv2Editor::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    //Reset current editor and model, clear caches.
    resetEditor();
    m_advancedModel.reset(new QStandardItemModel);
    QStringList advancedHeader;
    advancedHeader<<tr("Frame")<<tr("Frame Data");
    m_advancedModel->setHorizontalHeaderLabels(advancedHeader);
    m_advancedView->setModel(m_advancedModel.data());

    //Reset the media file, it might be used by other files.
    mediaFile.reset();
    //Use tag reader to read the tag. If tag exsist, display the data.
    if(m_tagAPEv2->readTag(mediaFile, mediaData))
    {
        setEditorText(Name   ,m_tagAPEv2->textData(KNMusicTagAPEv2::Name));
        setEditorText(Artist ,m_tagAPEv2->textData(KNMusicTagAPEv2::Artist));
        setEditorText(Album  ,m_tagAPEv2->textData(KNMusicTagAPEv2::Album));
        setEditorText(Year   ,m_tagAPEv2->textData(KNMusicTagAPEv2::Year));
        setEditorText(Track  ,m_tagAPEv2->textData(KNMusicTagAPEv2::Track));
        setEditorText(Genre  ,m_tagAPEv2->textData(KNMusicTagAPEv2::Genre));
        setEditorText(Comment,m_tagAPEv2->textData(KNMusicTagAPEv2::Comments));

        //Add all frames to advanced view.
        QStringList keyList=m_tagAPEv2->keyList();
        QStandardItem *currentItem;
        for(int i=0, keyListCount=keyList.size();
            i<keyListCount;
            i++)
        {
            //Create data row
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(keyList.at(i));
            currentRow.append(currentItem);
            currentItem=new QStandardItem(m_tagAPEv2->frameData(keyList.at(i)));
            currentRow.append(currentItem);
            //Append to model
            m_advancedModel->appendRow(currentRow);
        }
        m_advancedView->resizeColumnToContents(0);
    }
}

void KNMusicAPEv2Editor::resetEditor()
{
    //Reset the line text editor.
    for(int i=0; i<APEv2CaptionItemsCount; i++)
    {
        m_textEditor[i]->setText("");
    }
}

QString KNMusicAPEv2Editor::title() const
{
    //Return the name of the song.
    return m_tagAPEv2->textData(KNMusicTagAPEv2::Name);
}

QString KNMusicAPEv2Editor::album() const
{
    //Return the album of the song.
    return m_tagAPEv2->textData(KNMusicTagAPEv2::Album);
}

QString KNMusicAPEv2Editor::artist() const
{
    //Return the artist of the song.
    return m_tagAPEv2->textData(KNMusicTagAPEv2::Artist);
}

void KNMusicAPEv2Editor::retranslate()
{
    m_caption[Name]=tr("Name:");
    m_caption[Artist]=tr("Artist:");
    m_caption[Album]=tr("Album:");
    m_caption[Year]=tr("Year:");
    m_caption[Track]=tr("Track:");
    m_caption[Genre]=tr("Genre:");
    m_caption[Comment]=tr("Comment:");
}

void KNMusicAPEv2Editor::retranslateAndSet()
{
    ;
}

void KNMusicAPEv2Editor::writeTag(QFile &mediaFile, QDataStream &mediaData)
{
    m_tagAPEv2->clearCache();
    for(int i=0; i<m_advancedModel->rowCount(); i++)
    {
        m_tagAPEv2->setTextData(m_advancedModel->data(m_advancedModel->index(i,0),
                                                      Qt::DisplayRole).toString(),
                                m_advancedModel->data(m_advancedModel->index(i,1),
                                                      Qt::DisplayRole).toString());
    }
    m_tagAPEv2->writeTag(mediaFile, mediaData);
}

void KNMusicAPEv2Editor::toAdvancedMode()
{
    m_switcher->setCurrentIndex(1);
}

void KNMusicAPEv2Editor::toOverviewMode()
{
    m_switcher->setCurrentIndex(0);
}

void KNMusicAPEv2Editor::setTagData(const QString &key, const QString &data)
{
    //Search whether the frame exsists.
    QList<QStandardItem *> keyList=
            m_advancedModel->findItems(key, Qt::MatchExactly, 0);
    QStandardItem *currentItem;
    if(keyList.isEmpty())
    {
        if(data.isEmpty())
        {
            return;
        }
        //Create data row and extend it.
        QList<QStandardItem *> currentRow;
        currentItem=new QStandardItem(key);
        currentRow.append(currentItem);
        currentItem=new QStandardItem(data);
        currentRow.append(currentItem);
        //Append to model
        m_advancedModel->appendRow(currentRow);
    }
    else
    {
        if(data.isEmpty())
        {
            m_advancedModel->removeRow(keyList.first()->row());
            return;
        }
        //Replace the data
        m_advancedModel->setData(m_advancedModel->index(keyList.first()->row(),
                                                        1),
                                 data,
                                 Qt::DisplayRole);
    }
}

void KNMusicAPEv2Editor::onActionLineEditFinished(const int &index)
{
    QLineEdit *editor=static_cast<QLineEdit *>(m_editFinish->mapping(index));
    setTagData(m_tagAPEv2->frameAt(index), editor->text());
}

void KNMusicAPEv2Editor::setEditorText(const int &index, const QString &data)
{
    m_textEditor[index]->setText(data);
    m_textEditor[index]->setCursorPosition(0);
}
