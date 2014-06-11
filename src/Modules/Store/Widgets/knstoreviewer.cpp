#include "../Music/knmusicstore.h"

#include "knstoreviewer.h"

KNStoreViewer::KNStoreViewer(QWidget *parent) :
    KNStdLibViewer(parent)
{
    m_musicStore=new KNMusicStore(this);
    addCategory(QPixmap(),
                "Music",
                m_musicStore);
}
