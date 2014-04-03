#include <QDebug>

#include <QFileInfo>
#include "../knmusicglobal.h"

#include "knmusicsearcher.h"

KNMusicSearcher::KNMusicSearcher(QObject *parent) :
    QObject(parent)
{
    m_musicGlobal=KNMusicGlobal::instance();
}

void KNMusicSearcher::analysisList(const QList<QUrl> &urls)
{
    QString currentPath;
    QFileInfo currentDetails;
    int currentType;
    for(int i=0, fileCount=urls.count();
        i<fileCount;
        ++i)
    {
        currentPath=urls.at(i).path();
#ifdef Q_OS_WIN
        //Remove the very beginning '/' char.
        currentPath.remove(0, 1);
#endif
        currentDetails.setFile(currentPath);
        if(currentDetails.isDir())
        {
            //Search in the dir
            ;
        }
        if(currentDetails.isFile())
        {
            if(m_musicGlobal->getMusicType(currentDetails.suffix())!=-1)
            {
                emit requireAnalysis(currentPath);
            }
        }
    }
}
