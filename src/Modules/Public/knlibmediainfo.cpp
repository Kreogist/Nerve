#include "knlibmediainfo.h"

#include <QDebug>
#include <QStringList>

#include "MediaInfoDLL.h"
#define MediaInfoNameSpace MediaInfoDLL;
using namespace MediaInfoNameSpace;

#ifdef __MINGW32__
    #ifdef _UNICODE
        #define _itot _itow
    #else //_UNICODE
        #define _itot itoa
    #endif //_UNICODE
#endif //__MINGW32

KNLibMediaInfo::KNLibMediaInfo(QObject *parent) :
    QObject(parent)
{
    ;
}

void KNLibMediaInfo::analysisFile(const QString &filePath)
{
    //Information about MediaInfo
    MediaInfo MI;
    String To_Display;

    //An example of how to use the library
    MI.Open(filePath.toStdWString());

    MI.Option(__T("Complete"));
    To_Display += MI.Inform().c_str();

    MI.Close();

    m_originalData=QString::fromWCharArray(To_Display.c_str());
}

QString KNLibMediaInfo::originalData() const
{
    return m_originalData;
}
