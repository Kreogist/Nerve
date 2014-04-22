#include <QFileInfo>

#include "knmodel.h"
#include "knlibsearcher.h"

KNLibSearcher::KNLibSearcher(QObject *parent) :
    QObject(parent)
{
}

void KNLibSearcher::analysisList(const QList<QUrl> &urls)
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
            if(getType(currentDetails.suffix())!=-1)
            {
                emit requireAnalysis(currentPath);
            }
        }
    }
}

int KNLibSearcher::getType(const QString &suffix)
{
    Q_UNUSED(suffix);
    return -1;
}

KNModel *KNLibSearcher::model() const
{
    return m_model;
}

void KNLibSearcher::setModel(KNModel *model)
{
    m_model = model;
}

