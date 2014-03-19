#include <QDebug>

#include <QFileInfo>

#include "knmusicsearcher.h"

KNMusicSearcher::KNMusicSearcher(QObject *parent) :
    QObject(parent)
{
}

void KNMusicSearcher::analysisList(const QList<QUrl> &urls)
{
    QString currentPath;
    QFileInfo currentDetails;
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
            emit requireAnalysis(currentPath);
        }
    }
}
