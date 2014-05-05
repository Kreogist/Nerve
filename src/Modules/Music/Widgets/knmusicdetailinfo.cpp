#include <QMap>
#include <QTreeView>
#include <QStandardItem>
#include <QBoxLayout>

#include <QDebug>

#include "../../Public/knlibmediainfo.h"

#include "knmusicdetailinfo.h"

KNMusicDetailInfo::KNMusicDetailInfo(QWidget *parent) :
    QDialog(parent)
{
    QBoxLayout *tt=new QBoxLayout(QBoxLayout::TopToBottom, this);
    setLayout(tt);

    m_detailViewer=new QTreeView(this);
    tt->addWidget(m_detailViewer);
}

void KNMusicDetailInfo::setFilePath(const QString &filePath)
{
    QScopedPointer<KNLibMediaInfo> mediaInfo(new KNLibMediaInfo);
    mediaInfo->deepAnalysisFile(filePath);
    QString rawInfoData=mediaInfo->originalData(),
            currentData, currentIndex;
    if(rawInfoData.isEmpty())
    {
        return;
    }
    m_detailModel.reset(new QStandardItemModel);
    QMap<QString,QString> itemData;
    QStringList rawInfoItem;
#ifdef Q_OS_MACX
    rawInfoItem=rawInfoData.split('\r');
#endif
    int itemCount=rawInfoItem.size(),
        colonPosition;
    QStandardItem *currentItem, *currentParent;
    for(int i=0; i<itemCount; i++)
    {
        currentData=rawInfoItem.at(i);
        colonPosition=currentData.indexOf(":");
        if(colonPosition==-1)
        {
            //Means this is a header.
            currentParent=new QStandardItem(currentData.simplified());
            m_detailModel->appendRow(currentParent);
            continue;
        }
        currentIndex=currentData.left(colonPosition).simplified();
        if(currentIndex!="Cover_Data")
        {
            QList<QStandardItem *> currentRow;
            currentItem=new QStandardItem(currentIndex);
            currentRow.append(currentItem);
            currentItem=new QStandardItem(currentData.mid(colonPosition+1).simplified());
            currentRow.append(currentItem);
            m_detailModel->appendRow(currentRow);
            currentParent->setChild(i,
                                    m_detailModel->item(i));
        }
    }
    m_detailViewer->setModel(m_detailModel.data());
}
