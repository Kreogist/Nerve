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

void KNConfigure::setGroup(const QString &groupName)
{
    ;
}

KNConfigure::KNConfigure() :
    QObject()
{
    m_configurePath=QApplication::applicationDirPath()+"/configure.ini";
    m_settings=new QSettings(m_configurePath, QSettings::IniFormat, this);
}
