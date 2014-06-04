#include <QDir>
#include <QFile>
#include <QApplication>
#include <QFontDatabase>
#include <QFileInfo>
#include <QFileInfoList>

#include "knfontmanager.h"

KNFontManager *KNFontManager::m_instance=nullptr;

KNFontManager *KNFontManager::instance()
{
    return m_instance==nullptr?m_instance=new KNFontManager:m_instance;
}

KNFontManager::KNFontManager() :
    QObject()
{
    ;
}

void KNFontManager::loadCustomFont()
{
    QDir fontDir(QApplication::applicationDirPath()+"/Fonts");
    QFileInfoList fontFiles=fontDir.entryInfoList();
    int fontCount=fontFiles.size();
    while(fontCount--)
    {
        QFileInfo fontInfo=fontFiles.takeFirst();
        if(fontInfo.fileName()=="." || fontInfo.fileName()=="..")
        {
            continue;
        }
        if(fontInfo.isFile())
        {
            QFile fontFile(fontInfo.absoluteFilePath());
            if(fontFile.open(QIODevice::ReadOnly))
            {
                QFontDatabase::addApplicationFontFromData(fontFile.readAll());
                fontFile.close();
            }
        }
    }
}

void KNFontManager::setGlobalFont(const QString &familyName)
{
    QFont globalFont;
    globalFont.setFamily(familyName);
    globalFont.setPixelSize(m_fontSize);
    QApplication::setFont(globalFont);
}
