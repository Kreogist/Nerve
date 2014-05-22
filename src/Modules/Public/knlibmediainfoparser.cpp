#include <QDebug>

#include "knlibmediainfoparser.h"

KNLibMediaInfoParser::KNLibMediaInfoParser(QObject *parent) :
    QObject(parent)
{
}

void KNLibMediaInfoParser::parseData(const QString &rawInfoData)
{
    m_data.clear();
    QStringList rawInfoItem;
#ifdef Q_OS_WIN32
    rawInfoItem=rawInfoData.split('\n');
#endif
#ifdef Q_OS_MACX
    rawInfoItem=rawInfoData.split('\r');
#endif
#ifdef Q_OS_LINUX
    rawInfoItem=rawInfoData.split('\n');
#endif
    int itemCount=rawInfoItem.size(),
        colonPosition;
    QString currentRowData;
    MediaInfoBlock currentBlock;
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
            if(!currentBlock.name.isEmpty())
            {
                m_data.append(currentBlock);
            }
            currentBlock.name=currentRowData.simplified();
            currentBlock.key.clear();
            currentBlock.data.clear();
            continue;
        }
        currentBlock.key.append(currentRowData.left(colonPosition).simplified());
        currentBlock.data.append(currentRowData.mid(colonPosition+1).simplified());
    }
    m_data.append(currentBlock);
}

int KNLibMediaInfoParser::blockCount() const
{
    return m_data.size();
}

KNLibMediaInfoParser::MediaInfoBlock KNLibMediaInfoParser::blockAt(const int &index) const
{
    return m_data.at(index);
}

void KNLibMediaInfoParser::setCurrentBlock(const QString &blockName)
{
    for(int i=0; i<m_data.size(); i++)
    {
        if(m_data.at(i).name==blockName)
        {
            m_currentBlockIndex=i;
            return;
        }
    }
    m_currentBlockIndex=-1;
}

QString KNLibMediaInfoParser::data(const QString &key) const
{
    if(m_currentBlockIndex==-1)
    {
        return QString();
    }
    int keyIndex=m_data.at(m_currentBlockIndex).key.indexOf(key);
    return keyIndex==-1?QString():m_data.at(m_currentBlockIndex).data.at(keyIndex);
}
