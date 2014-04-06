#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QMimeData>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <QFileInfo>
#include <QUrl>

#include <QDebug>

#include "knglobal.h"

KNGlobal *KNGlobal::m_instance=nullptr;

KNGlobal *KNGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNGlobal:m_instance;
}

QString KNGlobal::byteToHigher(const qint64 &fileSize)
{
    qreal dFileSize=fileSize;
    int unitPointer=0;
    while(dFileSize>1024.0 && unitPointer<5)
    {
        dFileSize/=1024.0;
        unitPointer++;
    }
    return QString::number(dFileSize, 'f', 2) + " " + m_storageUnit[unitPointer];
}

KNGlobal::KNGlobal() :
    QObject()
{
    m_storageUnit[0]="Byte";
    m_storageUnit[1]="KB";
    m_storageUnit[2]="MB";
    m_storageUnit[3]="GB";
    m_storageUnit[4]="TB";
}

QWidget *KNGlobal::mainWindow() const
{
    return m_mainWindow;
}

void KNGlobal::setMainWindow(QWidget *mainWindow)
{
    m_mainWindow = mainWindow;
}

void KNGlobal::showInGraphicalShell(const QString &filePath)
{
#ifdef Q_OS_WIN32
    QStringList args;
    QFileInfo pathInfo(filePath);
    if(!pathInfo.isDir())
    {
        args<<"/select,";
    }
    args<<QDir::toNativeSeparators(filePath);
    QProcess::startDetached("explorer.exe",
                            args);
    return;
#endif
#ifdef Q_OS_MACX
    QStringList scriptArgs;
    scriptArgs << QLatin1String("-e")
               << QString::fromLatin1("tell application \"Finder\" to reveal POSIX file \"%1\"")
               .arg(filePath);
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    scriptArgs.clear();
    scriptArgs << QLatin1String("-e")
               << QLatin1String("tell application \"Finder\" to activate");
    QProcess::execute(QLatin1String("/usr/bin/osascript"), scriptArgs);
    return;
#endif
}

void KNGlobal::openLocalUrl(const QString &filePath)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

void KNGlobal::copyFileToClipboard(const QStringList &files)
{
    QMimeData fileData;
    QList<QUrl> fileUrlList;
    for(int i=0, fileCount=files.count();
        i<fileCount;
        i++)
    {
        QUrl currentUrl=QUrl::fromLocalFile(files.at(i));
        fileUrlList.append(currentUrl);
    }
    fileData.setUrls(fileUrlList);
    QApplication::clipboard()->setMimeData(&fileData);
}

void KNGlobal::copyTextToClipboard(const QString &text)
{
    QApplication::clipboard()->setText(text);
}
