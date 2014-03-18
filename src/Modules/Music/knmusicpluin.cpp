#include "../knglobal.h"

#include "Libraries/knmusicmodel.h"
#include "Libraries/knmusicinfocollector.h"
#include "Widgets/knmusicviewer.h"

#include "knmusicpluin.h"

KNMusicPluin::KNMusicPluin(QObject *parent) :
    KNPluginBase(parent)
{
    m_global=KNGlobal::instance();
    m_model=new KNMusicModel(this);

    m_musicViewer=new KNMusicViewer(m_global->mainWindow());
    m_musicViewer->setModel(m_model);

    m_inforCollector=new KNMusicInfoCollector(this);
    connect(m_inforCollector, SIGNAL(requireAppendMusic(QStringList)),
            m_model, SLOT(appendMusic(QStringList)));

    m_inforCollector->analysis("/Users/Saki/Music/カチューシャ（CV.金元寿子）＆ノンナ（CV.上坂すみれ） - カチューシャ.mp3");
    m_inforCollector->analysis("/Users/Saki/Music/fripSide - Only My Railgun.mp3");
    m_inforCollector->analysis("/Users/Saki/Music/fripSide - sister's noise.mp3");
    m_inforCollector->analysis("/Users/Saki/Music/Kimeru - Overlap (第5期opテーマ) .mp3");
    m_inforCollector->analysis("/Users/Saki/Music/LOU GRANT - DON'T STOP THE MUSIC.mp3");
    m_inforCollector->analysis("/Users/Saki/Music/nomico - Bad Apple!!.mp3");
    m_inforCollector->analysis("/Users/Saki/Music/RO-KYU-BU! - SHOOT！.mp3");
    m_inforCollector->analysis("/Users/Saki/Music/senya - 泡沫、哀のまほろば.mp3");
    m_inforCollector->analysis("/Users/Saki/Music/StylipS - MIRACLE RUSH.mp3");
}

void KNMusicPluin::applyPlugin()
{
    emit requireAddCategory("test",
                            QPixmap(),
                            m_musicViewer);
}
