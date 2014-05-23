#include <QTreeView>
#include <QStandardItem>
#include <QBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QHeaderView>
#include <QFileInfo>
#include <QDateTime>
#include <QPlainTextEdit>

#include <QDebug>

#include "../../Public/knlibmediainfo.h"
#include "../../Public/knlibmediainfoparser.h"
#include "../../Base/knstdlibviewer.h"
#include "knmusictageditor.h"

#include "knmusicdetailinfo.h"

KNMusicDetailOverview::KNMusicDetailOverview(QWidget *parent) :
    QWidget(parent)
{
    retranslate();
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(mainLayout);
    for(int i=0; i<MusicBasicCount; i++)
    {
        m_basicInfo[i]=new QLabel(this);
        mainLayout->addWidget(m_basicInfo[i]);
    }

    QFormLayout *detailsLayout=new QFormLayout(mainLayout->widget());
    detailsLayout->setLabelAlignment(Qt::AlignRight);
    mainLayout->addLayout(detailsLayout);

    for(int i=0; i<MusicDetailCount; i++)
    {
        m_labels[i]=new QLabel(m_caption[i], this);
        QFont labelFont=m_labels[i]->font();
        labelFont.setBold(true);
        m_labels[i]->setFont(labelFont);
        m_datas[i]=new QLabel(this);
        detailsLayout->addRow(m_labels[i], m_datas[i]);
    }
}

void KNMusicDetailOverview::setText(const int &index, const QString &text)
{
    m_labels[index]->setVisible(!text.isEmpty());
    m_datas[index]->setVisible(!text.isEmpty());
    m_datas[index]->setText(text);
}

void KNMusicDetailOverview::setName(const QString &name)
{
    m_datas[Name]->setText(name);
}

void KNMusicDetailOverview::setBasicInfo(const int &index, const QString &basicData)
{
    m_basicInfo[index]->setVisible(!basicData.isEmpty());
    m_basicInfo[index]->setText(basicData);
}

void KNMusicDetailOverview::retranslate()
{
    m_caption[Kind]=tr("Kind:");
    m_caption[Size]=tr("Size:");
    m_caption[Duration]=tr("Duration:");
    m_caption[Format]=tr("Format:");
    m_caption[BitRate]=tr("Bit Rate:");
    m_caption[SampleRate]=tr("Sample Rate:");
    m_caption[Channels]=tr("Channel(s):");
    m_caption[CompressionMode]=tr("Compression Mode:");
    m_caption[WritingLibrary]=tr("Encoded with:");
    m_caption[DateModified]=tr("Date Modified:");
    m_caption[LastPlayed]=tr("Last Played:");
}

void KNMusicDetailOverview::retranslateAndSet()
{
    ;
}

KNMusicDetailInfo::KNMusicDetailInfo(QWidget *parent) :
    QDialog(parent)
{
    setContentsMargins(0,0,0,0);

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    m_switcher=new KNStdLibViewer(this);
    mainLayout->addWidget(m_switcher);

    m_parser=new KNLibMediaInfoParser(this);

    m_overall=new KNMusicDetailOverview(this);
    m_switcher->addCategory(QPixmap(":/Music/Resources/Music/details/Overall.png"), "Overall", m_overall);

    m_tagEditor=new KNMusicTagEditor(this);
    m_switcher->addCategory(QPixmap(":/Music/Resources/Music/details/Tag.png"), "Tag", m_tagEditor);

    m_detailViewer=new QTreeView(this);
    m_detailViewer->setAnimated(true);
    m_detailViewer->setAllColumnsShowFocus(true);
    m_switcher->addCategory(QPixmap(":/Music/Resources/Music/details/Treeview.png"), "Details", m_detailViewer);

    m_detailText=new QPlainTextEdit(this);
    m_detailText->setWordWrapMode(QTextOption::NoWrap);
    m_switcher->addCategory(QPixmap(":/Music/Resources/Music/details/Textview.png"), "Text", m_detailText);
}

void KNMusicDetailInfo::setFilePath(const QString &filePath)
{
    //Reset Media Info Module
    QScopedPointer<KNLibMediaInfo> mediaInfo(new KNLibMediaInfo);
    QFileInfo fileInfo(filePath);
    mediaInfo->analysisFile(filePath);
    //Get Media Info data, if no data returned, means there's some bug happend.
    QString rawInfoData=mediaInfo->originalData();
    if(rawInfoData.isEmpty())
    {
        return;
    }
    //Use MediaInfo data parser to parse the data.
    m_parser->parseData(rawInfoData);
    //Add datas to the treeview and textview.
    int blockCount=m_parser->blockCount();
    KNLibMediaInfoParser::MediaInfoBlock currentBlock;
    QString textData;
    QStandardItem *currentItem, *currentParent;
    //Reset the detail model.
    m_detailModel.reset(new QStandardItemModel);
    for(int i=0; i<blockCount; i++)
    {
        //Get one block
        currentBlock=m_parser->blockAt(i);
        QList<QStandardItem *> parentRow;
        currentParent=new QStandardItem(currentBlock.name);
        parentRow.append(currentParent);
        currentItem=new QStandardItem();
        parentRow.append(currentItem);
        m_detailModel->appendRow(parentRow);
        //Append block name to text data string.
        textData+=currentBlock.name+"\n";
        for(int j=0; j<currentBlock.key.size(); j++)
        {
            //Append a block row to treeview model.
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(currentBlock.key.at(j));
            currentRow.append(currentItem);
            currentItem=new QStandardItem(currentBlock.data.at(j));
            currentRow.append(currentItem);
            currentParent->appendRow(currentRow);
            //Append the row data to text string
            textData+=currentBlock.key.at(j)+" : "+
                      currentBlock.data.at(j)+"\n";
        }
        textData+="\n";
    }
    m_detailViewer->setModel(m_detailModel.data());
    m_detailViewer->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_detailViewer->header()->setVisible(false);
    m_detailViewer->expandAll();
    m_detailText->setPlainText(textData);

    //Set data to overview.
    m_parser->setCurrentBlock("General");
    m_overall->setBasicInfo(KNMusicDetailOverview::Duration, m_parser->data("Duration"));
    m_overall->setText(KNMusicDetailOverview::Size, m_parser->data("File size"));
    m_overall->setText(KNMusicDetailOverview::Kind, m_parser->data("Format"));
    m_overall->setText(KNMusicDetailOverview::BitRate, m_parser->data("Overall bit rate"));

    m_parser->setCurrentBlock("Audio");
    m_overall->setText(KNMusicDetailOverview::Format, m_parser->data("Format")+" "+
                                                      m_parser->data("Format version")+" "+
                                                      m_parser->data("Format profile"));
    m_overall->setText(KNMusicDetailOverview::SampleRate, m_parser->data("Sampling rate"));
    m_overall->setText(KNMusicDetailOverview::CompressionMode, m_parser->data("Compression mode"));
    m_overall->setText(KNMusicDetailOverview::WritingLibrary, m_parser->data("Writing library"));
    QString channelCount=m_parser->data("Channel(s)"),
            channelMode=m_parser->data("Mode"),
            channelData;
    if(channelCount.isEmpty())
    {
        if(!channelMode.isEmpty())
        {
            channelData=channelMode;
        }
    }
    else
    {
        channelData=channelMode.isEmpty()?
                    channelCount:channelCount+"("+channelMode+")";
    }
    m_overall->setText(KNMusicDetailOverview::Channels, channelData);
    m_overall->setText(KNMusicDetailOverview::DateModified,
                       fileInfo.lastModified().toString("yyyy-MMMM-dd, HH:mm AP"));
    m_overall->setText(KNMusicDetailOverview::LastPlayed,
                       fileInfo.lastRead().toString("yyyy-MMMM-dd, HH:mm AP"));

    //Ask tag editor to parse the file, set the file data to overall view.
    m_tagEditor->parseFile(filePath);
    m_overall->setBasicInfo(KNMusicDetailOverview::Name, m_tagEditor->title());
    m_overall->setBasicInfo(KNMusicDetailOverview::Artist, m_tagEditor->artist());
    m_overall->setBasicInfo(KNMusicDetailOverview::Album, m_tagEditor->album());
}
