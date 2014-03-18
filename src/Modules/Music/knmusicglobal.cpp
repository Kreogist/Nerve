#include "knmusicglobal.h"

KNMusicGlobal *KNMusicGlobal::m_instance=nullptr;

KNMusicGlobal *KNMusicGlobal::instance()
{
    return m_instance==nullptr?m_instance=new KNMusicGlobal:m_instance;
}

KNMusicGlobal::KNMusicGlobal() :
    QObject()
{
    ;
}
