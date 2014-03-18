#include "../knglobal.h"

#include "Widgets/knmusicviewer.h"

#include "knmusicpluin.h"

KNMusicPluin::KNMusicPluin(QObject *parent) :
    KNPluginBase(parent)
{
    m_global=KNGlobal::instance();
    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
}

void KNMusicPluin::applyPlugin()
{
    emit requireAddCategory("test",
                            QPixmap(),
                            m_musicViewer);
}
