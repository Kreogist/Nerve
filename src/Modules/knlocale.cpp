#include "knlocale.h"

KNLocale *KNLocale::m_instance=nullptr;

KNLocale *KNLocale::instance()
{
    return m_instance==nullptr?m_instance=new KNLocale:m_instance;
}

KNLocale::KNLocale() :
    QObject()
{
}
