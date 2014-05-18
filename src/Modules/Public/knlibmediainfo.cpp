#include "knlibmediainfo.h"

#include <QDebug>
#include <QFile>
#include <QStringList>

#ifdef Q_OS_WIN32
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
#endif

#ifdef Q_OS_MACX
#include "MediaInfoDLL.h"
#define MediaInfoNameSpace MediaInfoDLL;
using namespace MediaInfoNameSpace;
#endif

#ifdef Q_OS_LINUX
#include <string>
#include "ZenLib/Ztring.h"
#include "MediaInfoDLL/MediaInfoDLL.h"
#define MediaInfoNameSpace MediaInfoDLL
using namespace MediaInfoNameSpace;
using namespace ZenLib;
#endif

KNLibMediaInfo::KNLibMediaInfo(QObject *parent) :
    QObject(parent)
{
    ;
}

void KNLibMediaInfo::analysisFile(const QString &filePath)
{
#ifdef Q_OS_WIN32
    MediaInfo MI;
    MI.Open(filePath.toStdWString());
    MI.Option(__T("Complete"));
    m_originalData=QString::fromWCharArray(MI.Inform().c_str());
    MI.Close();
#endif
#ifdef Q_OS_UNIX
    MediaInfo MI;
    MI.Open(filePath.toStdString().c_str());
    MI.Option("Inform", "");
    MI.Option("Complete", "0");
    m_originalData=MI.Inform().c_str();
    MI.Close();
#endif
}

void KNLibMediaInfo::deepAnalysisFile(const QString &filePath)
{
#ifdef Q_OS_WIN32
    MediaInfo MI;
    MI.Open(filePath.toStdWString());
    MI.Option(__T("Complete"));
    m_originalData=QString::fromWCharArray(MI.Inform().c_str());
    MI.Close();
#endif
#ifdef Q_OS_UNIX
    MediaInfo MI;
    MI.Open(filePath.toStdString().c_str());
    MI.Option("Complete", "1");
    m_originalData=MI.Inform().c_str();
    MI.Close();
#endif
}

void KNLibMediaInfo::quickAnalysisFile(const QString &filePath)
{
    MediaInfo MI;
#ifdef Q_OS_WIN32
    MI.Open(filePath.toStdWString());
    MI.Option(__T("inform"), __T("Audio;%Duration%"));
    m_originalData=QString::fromWCharArray(MI.Inform().c_str())+"\n";
    MI.Option(__T("inform"), __T("Audio;%BitRate%"));
    m_originalData+=QString::fromWCharArray(MI.Inform().c_str())+"\n";
    MI.Option(__T("inform"), __T("Audio;%SamplingRate%"));
    m_originalData+=QString::fromWCharArray(MI.Inform().c_str());
#endif
#ifdef Q_OS_UNIX
    MI.Open(filePath.toStdString().c_str());
    MI.Option("inform", "Audio;%Duration%");
    m_originalData=QString(MI.Inform().c_str())+"\n";
    MI.Option("inform", "Audio;%BitRate%");
    m_originalData+=QString(MI.Inform().c_str())+"\n";
    MI.Option("inform", "Audio;%SamplingRate%");
    m_originalData+=MI.Inform().c_str();
#endif
    MI.Close();
}

QString KNLibMediaInfo::originalData() const
{
    return m_originalData;
}
