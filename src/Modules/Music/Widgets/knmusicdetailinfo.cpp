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
    QFormLayout *detailsLayout=new QFormLayout(this);
    detailsLayout->setLabelAlignment(Qt::AlignRight);
    setLayout(detailsLayout);

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
    m_datas[index]->setText(text);
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
    m_detailModel.reset(new QStandardItemModel);
    for(int i=0; i<blockCount; i++)
    {
        currentBlock=m_parser->blockAt(i);
        QList<QStandardItem *> parentRow;
        currentParent=new QStandardItem(currentBlock.name);
        parentRow.append(currentParent);
        currentItem=new QStandardItem();
        parentRow.append(currentItem);
        m_detailModel->appendRow(parentRow);
        textData+=currentBlock.name+"\n";
        for(int j=0; j<currentBlock.key.size(); j++)
        {
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(currentBlock.key.at(j));
            currentRow.append(currentItem);
            currentItem=new QStandardItem(currentBlock.data.at(j));
            currentRow.append(currentItem);
            currentParent->appendRow(currentRow);
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
    m_overall->setText(KNMusicDetailOverview::Size, m_parser->data("File size"));
    m_overall->setText(KNMusicDetailOverview::Kind, m_parser->data("Format"));
    m_overall->setText(KNMusicDetailOverview::Duration, m_parser->data("Duration"));
    m_overall->setText(KNMusicDetailOverview::BitRate, m_parser->data("Overall bit rate"));

    m_parser->setCurrentBlock("Audio");
    m_overall->setText(KNMusicDetailOverview::Format, m_parser->data("Format")+" "+
                                                      m_parser->data("Format version")+" "+
                                                      m_parser->data("Format profile"));
    m_overall->setText(KNMusicDetailOverview::SampleRate, m_parser->data("Sampling rate"));
    m_overall->setText(KNMusicDetailOverview::CompressionMode, m_parser->data("Compression mode"));
    m_overall->setText(KNMusicDetailOverview::WritingLibrary, m_parser->data("Writing library"));
    m_overall->setText(KNMusicDetailOverview::Channels, m_parser->data("Channel(s)")+"("+
                                                        m_parser->data("Mode")+
                                                        ")");
    m_overall->setText(KNMusicDetailOverview::DateModified,
                       fileInfo.lastModified().toString("yyyy-MMMM-dd, HH:mm AP"));
    m_overall->setText(KNMusicDetailOverview::LastPlayed,
                       fileInfo.lastRead().toString("yyyy-MMMM-dd, HH:mm AP"));
}
