#include <QSettings>
#include <QApplication>

#include "knconfigure.h"

KNConfigure *KNConfigure::m_instance=nullptr;

KNConfigure *KNConfigure::instance()
{
    return m_instance==nullptr?m_instance=new KNConfigure:m_instance;
}

void KNConfigure::setValue(const QString &key,
                           const QVariant &value)
{
    ;
}

void KNConfigure::setValue(const QString &groupName,
                           const QString &key,
                           const QVariant &value)
{
    ;
}

QVariant KNConfigure::value(const QString &key)
{
    return m_settings->value(key);
}

QVariant KNConfigure::value(const QString &groupName,
                            const QString &key)
{
    ;
}

void KNConfigure::beginGroup(const QString &groupName)
{
    m_settings->beginGroup(groupName);
}

void KNConfigure::endGroup()
{
    m_settings->endGroup();
}

KNConfigure::KNConfigure() :
    QObject()
{
    m_configurePath=QApplication::applicationDirPath()+"/configure.ini";
    m_settings=new QSettings(m_configurePath, QSettings::IniFormat, this);
}
