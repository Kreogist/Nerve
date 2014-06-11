#include <QBoxLayout>

#include "../../Base/knopacitybutton.h"
#include "../../Base/knmenu.h"

#include "knmusicplaylistlisteditor.h"

KNMusicPlaylistListEditor::KNMusicPlaylistListEditor(QWidget *parent) :
    QWidget(parent)
{
    //Get latest translation.
    retranslate();

    //Set properties.
    setContentsMargins(0,0,0,0);
    setAutoFillBackground(true);
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(7,7,7));
    setPalette(pal);

    //Set buttons.
    m_toolsLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_toolsLayout->setSpacing(0);
    setLayout(m_toolsLayout);
    m_createList=new KNOpacityButton(this);
    m_createList->setPixmap(QPixmap(":/Music/Resources/Music/playlists/add.png"));
    m_toolsLayout->addWidget(m_createList);
    m_removeList=new KNOpacityButton(this);
    m_removeList->setPixmap(QPixmap(":/Music/Resources/Music/playlists/remove.png"));
    m_toolsLayout->addWidget(m_removeList);
    m_toolsLayout->addStretch();

    //Initial menu.
    m_itemAdd=new KNMenu(this);
    createActions();
}

void KNMusicPlaylistListEditor::retranslate()
{
    m_itemAddCaption[AddPlaylist]=tr("New Playlist");
}

void KNMusicPlaylistListEditor::retranslateAndSet()
{
    ;
}

void KNMusicPlaylistListEditor::createActions()
{
    for(int i=0; i<ActionAddCount; i++)
    {
        m_itemAddAction[i]=new QAction(m_itemAddCaption[i], this);
        m_itemAdd->addAction(m_itemAddAction[i]);
    }
    connect(m_itemAddAction[AddPlaylist], SIGNAL(triggered()),
            this, SIGNAL(requireCreatePlaylist()));
    connect(m_createList, &KNOpacityButton::clicked,
            [=]
            {
                m_itemAdd->exec(mapToGlobal(m_createList->pos()));
            });
    connect(m_removeList, &KNOpacityButton::clicked,
            this, &KNMusicPlaylistListEditor::requireRemoveCurrent);
}
