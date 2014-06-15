#include <QDir>
#include <QFileInfoList>

#include <QDebug>

#include "knmodel.h"
#include "knlibfilter.h"

KNLibFilter::KNLibFilter(QObject *parent) :
    QObject(parent)
{
}

void KNLibFilter::analysisUrls(const QList<QUrl> &urls)
{
    m_rawFileList.clear();
    QFileInfo currentParser;
    QString currentPath;
    for(int i=0, fileCount=urls.size();
        i<fileCount;
        ++i)
    {
        currentPath=urls.at(i).path();
#ifdef Q_OS_WIN32
        //Remove the very beginning '/' char.
        currentPath.remove(0, 1);
#endif
        currentParser.setFile(currentPath);
        if(currentParser.isDir())
        {
            //Search in the dir
            analysisFolder(currentParser.absoluteFilePath());
        }
        if(currentParser.isFile())
        {
            analysisFile(currentParser.suffix(),
                         currentPath);
        }
    }
    emit requireAddRawFiles(m_rawFileList);
    m_rawFileList.clear();
}

int KNLibFilter::getType(const QString &suffix)
{
    Q_UNUSED(suffix);
    return -1;
}

void KNLibFilter::analysisFolder(const QString &folderPath)
{
    QDir folderInfo(folderPath);
    QFileInfoList filesData=folderInfo.entryInfoList();
    QFileInfo currentParser;
    QString currentName;
    for(int i=0; i<filesData.size(); i++)
    {
        currentParser=filesData.at(i);
        currentName=currentParser.fileName();
        if(currentName=="." || currentName=="..")
        {
            continue;
        }
        if(currentParser.isFile())
        {
            analysisFile(currentParser.suffix(),
                         currentParser.absoluteFilePath());
        }
        if(currentParser.isDir())
        {
            analysisFolder(currentParser.absoluteFilePath());
        }
    }
}

void KNLibFilter::analysisFile(const QString &suffix,
                                 const QString &filePath)
{
    if(getType(suffix)!=-1)
    {
        m_rawFileList.append(filePath);
    }
}

KNModel *KNLibFilter::model() const
{
    return m_model;
}

void KNLibFilter::setModel(KNModel *model)
{
    m_model = model;
}

