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
    return QString::number(dFileSize, 'g', 2) + m_storageUnit[unitPointer];
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

