#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <QDebug>

#include "knlibdatabase.h"

KNLibDatabase::KNLibDatabase(QObject *parent) :
    QObject(parent)
{
    m_databaseFile=new QFile(this);
    m_document=QJsonDocument(m_content);
}

void KNLibDatabase::setDatabase(const QString &filePath)
{
    m_databaseFileInfo=QFileInfo(filePath);
    m_databaseFilePath=m_databaseFileInfo.absoluteFilePath();
    m_databaseFile->setFileName(m_databaseFilePath);
    if(m_databaseFileInfo.exists())
    {
        readDatabase();
    }
    else
    {
        createDatabase();
    }
}

void KNLibDatabase::replace(int i, const QJsonValue &value)
{
    m_databaseArray.replace(i, value);
}

QJsonValue KNLibDatabase::at(int i) const
{
    return m_databaseArray.at(i);
}

int KNLibDatabase::size() const
{
    return m_databaseArray.size();
}

void KNLibDatabase::clearCache()
{
    m_databaseArray=QJsonArray();
}

void KNLibDatabase::readDatabase()
{
    clearCache();
    m_databaseFile->open(QIODevice::ReadOnly);
    QByteArray readCache=m_databaseFile->readAll();
    m_databaseFile->close();

    m_document=QJsonDocument::fromJson(readCache);
    m_content=m_document.object();
    m_databaseArray=m_content["Database"].toArray();
    if(m_content["MajorVersion"].toInt()>m_majorVersion ||
       m_content["MinorVersion"].toInt()>m_minorVersion)
    {
        qWarning()<<"Created by a higher version.";
    }
}

void KNLibDatabase::writeDatabase()
{
    m_content["Database"]=m_databaseArray;
    m_document.setObject(m_content);
    m_databaseFile->open(QIODevice::WriteOnly);
    m_databaseFile->write(m_document.toJson());
    m_databaseFile->close();
}

void KNLibDatabase::createDatabase()
{
    QDir databaseDir=m_databaseFileInfo.absoluteDir();
    databaseDir.mkpath(databaseDir.absolutePath());
    if(!m_databaseFile->open(QIODevice::WriteOnly))
    {
        //Failed
        return;
    }
    m_databaseFile->close();
    m_content["MajorVersion"]=m_majorVersion;
    m_content["MinorVersion"]=m_minorVersion;
    writeDatabase();
}
