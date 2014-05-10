#include <QMap>
#include <QTreeView>
#include <QStandardItem>
#include <QBoxLayout>
#include <QHeaderView>
#include <QPlainTextEdit>
#include <QTabWidget>

#include <QDebug>

#include "../../Public/knlibmediainfo.h"

#include "knmusicdetailinfo.h"

KNMusicDetailInfo::KNMusicDetailInfo(QWidget *parent) :
    QDialog(parent)
{
    QBoxLayout *tt=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(tt);

    QTabWidget *tw=new QTabWidget(this);
    tt->addWidget(tw);

    m_detailViewer=new QTreeView(this);
    tw->addTab(m_detailViewer,"TreeView");

    m_detailText=new QPlainTextEdit(this);
    m_detailText->setWordWrapMode(QTextOption::NoWrap);
    tw->addTab(m_detailText, "TextView");
}

void KNMusicDetailInfo::setFilePath(const QString &filePath)
{
    QScopedPointer<KNLibMediaInfo> mediaInfo(new KNLibMediaInfo);
    mediaInfo->analysisFile(filePath);
    QString rawInfoData=mediaInfo->originalData(),
            currentRowData, currentIndex, currentData;
    if(rawInfoData.isEmpty())
    {
        return;
    }
    m_detailText->setPlainText(rawInfoData);
    m_detailModel.reset(new QStandardItemModel);
    QMap<QString,QString> itemData;
    QStringList rawInfoItem;
#ifdef Q_OS_WIN32
    rawInfoItem=rawInfoData.split('\n');
#endif
#ifdef Q_OS_MACX
    rawInfoItem=rawInfoData.split('\r');
#endif
    int itemCount=rawInfoItem.size(),
        colonPosition;
    QStandardItem *currentItem, *currentParent;
    for(int i=0; i<itemCount; i++)
    {
        currentRowData=rawInfoItem.at(i);
        if(currentRowData.isEmpty())
        {
            continue;
        }
        colonPosition=currentRowData.indexOf(":");
        if(colonPosition==-1)
        {
            //Means this is a header.
            QList<QStandardItem *> currentRow;
            currentParent=new QStandardItem(currentRowData.simplified());
            currentRow.append(currentParent);
            currentItem=new QStandardItem();
            currentRow.append(currentItem);
            m_detailModel->appendRow(currentRow);
            continue;
        }
        QList<QStandardItem *> currentRow;
        currentIndex=currentRowData.left(colonPosition).simplified();
        currentItem=new QStandardItem(currentIndex);
        currentRow.append(currentItem);
        currentData=currentRowData.mid(colonPosition+1).simplified();
        currentItem=new QStandardItem(currentData);
        currentRow.append(currentItem);
        currentParent->appendRow(currentRow);
        itemData[currentIndex]=currentData;
    }
    m_detailViewer->setModel(m_detailModel.data());
    m_detailViewer->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_detailViewer->header()->setVisible(false);
    m_detailViewer->expandAll();
}
