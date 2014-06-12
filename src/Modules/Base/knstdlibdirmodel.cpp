#include "knstdlibdirmodel.h"

KNStdLibDirModel::KNStdLibDirModel(QObject *parent) :
    KNLibDirModel(parent)
{
    m_currentDir=new QDir(QDir::homePath());
}

void KNStdLibDirModel::refresh()
{
    clear();
    QFileInfoList currentList=m_currentDir->entryInfoList(QDir::AllEntries |
                                                          QDir::NoDotAndDotDot |
                                                          QDir::Hidden |
                                                          QDir::System |
                                                          QDir::CaseSensitive);
    int listCount=currentList.size();
    while(listCount--)
    {
        QFileInfo currentInfo=currentList.takeFirst();
//        ;
    }
}
