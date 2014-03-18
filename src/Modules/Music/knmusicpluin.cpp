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

    m_inforCollector->analysis("D:/01. Won(＊3＊)Chu KissMe!_FullID3v2.mp3");
    /*m_inforCollector->analysis("D:/音乐/Kimeru - Overlap (第5期opテーマ) .mp3");
    m_inforCollector->analysis("D:/音乐/LOU GRANT - DON'T STOP THE MUSIC.mp3");
    m_inforCollector->analysis("D:/音乐/nomico - Bad Apple!!.mp3");
    m_inforCollector->analysis("D:/音乐/RO-KYU-BU! - SHOOT！.mp3");
    m_inforCollector->analysis("D:/音乐/senya - 泡沫、哀のまほろば.mp3");
    m_inforCollector->analysis("D:/音乐/StylipS - MIRACLE RUSH.mp3");*/
}

void KNMusicPluin::applyPlugin()
{
    emit requireAddCategory("test",
                            QPixmap(),
                            m_musicViewer);
}
