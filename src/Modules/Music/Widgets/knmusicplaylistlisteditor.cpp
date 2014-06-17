#include <QBoxLayout>

#include "../../Base/knopacitybutton.h"

#include "knmusicplaylistlisteditor.h"

KNMusicPlaylistListEditor::KNMusicPlaylistListEditor(QWidget *parent) :
    QWidget(parent)
{
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(mainLayout);

    m_add=new KNOpacityButton(this);
    m_add->setPixmap(QPixmap(":/Music/Resources/Music/playlists/add.png"));
    mainLayout->addWidget(m_add);
    m_remove=new KNOpacityButton(this);
    m_remove->setPixmap(QPixmap(":/Music/Resources/Music/playlists/remove.png"));
    mainLayout->addWidget(m_remove);
    mainLayout->addStretch();
    m_edit=new KNOpacityButton(this);
    m_edit->setPixmap(QPixmap(":/Music/Resources/Music/playlists/edit.png"));
    mainLayout->addWidget(m_edit);
}
