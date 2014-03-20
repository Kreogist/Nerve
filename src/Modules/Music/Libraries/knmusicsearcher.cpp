#include <QDebug>

#include <QFileInfo>

#include "knmusicsearcher.h"

KNMusicSearcher::KNMusicSearcher(QObject *parent) :
    QObject(parent)
{
    m_suffixs<<"mp3"
             <<"m4a"
             <<"wma"
             <<"wav"
             <<"flac"
             <<"ape"
             <<"ogg"
             <<"tta"
             <<"aiff"
             <<"aifc"
             <<"mp3pro"
             <<"fla"
             <<"tak"
             <<"aif"
             <<"aac"
             <<"cda"
             <<"mp+"
             <<"mp4"
             <<"mpa"
             <<"mp2"
             <<"mp1"
             <<"midi"
             <<"mid"
             <<"mpc"
             <<"aa"
             <<"ra"
             <<"mac"
             <<"rmi"
             <<"dtswav"
             <<"dts"
             <<"cur"
             <<"snd"
             <<"au"
             <<"ac3"
             <<"as2"
             <<"xm"
             <<"umx"
             <<"stm"
             <<"s3m"
             <<"mtm"
             <<"mod"
             <<"it"
             <<"far";
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
            if(m_suffixs.indexOf(currentDetails.suffix())!=-1)
            {
                emit requireAnalysis(currentPath);
            }
        }
    }
}
