#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonParseError>

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
}

void KNLibDatabase::load()
{
    if(m_databaseFileInfo.exists())
    {
        if(readFromDisk())
        {
            return;
        }
    }
    createDatabase();
}

void KNLibDatabase::replace(int i, const QJsonValue &value)
{
    m_databaseArray.replace(i, value);
}

void KNLibDatabase::append(const QJsonValue &value)
{
    m_databaseArray.append(value);
}

QJsonValue KNLibDatabase::takeAt(int i)
{
    return m_databaseArray.takeAt(i);
}

void KNLibDatabase::removeAt(int i)
{
    return m_databaseArray.removeAt(i);
}

QJsonValue KNLibDatabase::at(int i) const
{
    return m_databaseArray.at(i);
}

QJsonObject KNLibDatabase::row(int i) const
{
    return m_databaseArray.at(i).toObject();
}

int KNLibDatabase::size() const
{
    return m_databaseArray.size();
}

void KNLibDatabase::clearCache()
{
    m_databaseArray=QJsonArray();
}

bool KNLibDatabase::readFromDisk()
{
    clearCache();
    m_databaseFile->open(QIODevice::ReadOnly);
    QByteArray readCache=m_databaseFile->readAll();
    m_databaseFile->close();

    qDebug()<<readCache.size();
    QJsonParseError error;
    m_document=QJsonDocument::fromJson(readCache, &error);
    qDebug()<<error.errorString();

    if(m_document.isNull())
    {
        return false;
    }
    m_content=m_document.object();
    m_databaseArray=m_content["Database"].toArray();
    if(m_content["MajorVersion"].toInt()>m_majorVersion ||
       m_content["MinorVersion"].toInt()>m_minorVersion)
    {
        qWarning()<<"Created by a higher version.";
    }
    return true;
}

void KNLibDatabase::writeToDisk()
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
    m_content.insert("MajorVersion", m_majorVersion);
    m_content.insert("MinorVersion", m_minorVersion);
    writeToDisk();
}
