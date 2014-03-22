#include <QProcess>
#include <QDir>
#include <QStringList>

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
#ifdef Q_OS_WIN
    QProcess::startDetached("explorer.exe",
                            QStringList(QString(QLatin1String("/select,") +
                                        QDir::toNativeSeparators(filePath))));
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


