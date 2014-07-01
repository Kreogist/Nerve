#include <QAction>
#include <QResizeEvent>
#include <QTimeLine>
#include <QLinearGradient>
#include <QBoxLayout>

#include "../../Base/knopacitybutton.h"
#include "../../Base/knmenu.h"

#include "knmusicplaylistlisteditor.h"

KNMusicPlaylistListEditor::KNMusicPlaylistListEditor(QWidget *parent) :
    KNMusicPlaylistListEditorBase(parent)
{
    //Get translation
    retranslate();

    //Set properties.
    setAutoFillBackground(true);

    //Set layout.
    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(mainLayout);

    //Set palette.
    m_palette=palette();
    m_background=QLinearGradient(QPoint(0,0), QPoint(0, height()));
    m_backgroundColor=QColor(0x27,0x27,0x27);
    m_background.setColorAt(0, m_backgroundColor);
    m_background.setColorAt(1, m_backgroundColor);
    m_palette.setBrush(QPalette::Window, QBrush(m_background));
    setPalette(m_palette);

    //Set buttons.
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

    //Create add menu.
    createAddMenu();
    connect(m_add, &KNOpacityButton::mouseReleased,
            this, &KNMusicPlaylistListEditor::showAddMenu);

    //Set enter and leave animation.
    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x4e);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistListEditor::changeBackground);

    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(0x27);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicPlaylistListEditor::changeBackground);
}

void KNMusicPlaylistListEditor::retranslate()
{
    m_addActionCaption[Playlist]=tr("New Playlist");
}

void KNMusicPlaylistListEditor::retranslateAndSet()
{
    retranslate();
}

void KNMusicPlaylistListEditor::enterEvent(QEvent *event)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_mouseIn->setStartFrame(m_backgroundColor.red());
    m_mouseIn->start();
    KNMusicPlaylistListEditorBase::enterEvent(event);
}

void KNMusicPlaylistListEditor::leaveEvent(QEvent *event)
{
    m_mouseIn->stop();
    m_mouseOut->stop();
    m_mouseOut->setStartFrame(m_backgroundColor.red());
    m_mouseOut->start();
    KNMusicPlaylistListEditorBase::leaveEvent(event);
}

void KNMusicPlaylistListEditor::resizeEvent(QResizeEvent *event)
{
    KNMusicPlaylistListEditorBase::resizeEvent(event);
    m_background.setFinalStop(QPoint(0,height()));
    m_palette.setBrush(QPalette::Window, QBrush(m_background));
    setPalette(m_palette);
}

void KNMusicPlaylistListEditor::changeBackground(const int &frame)
{
    m_backgroundColor=QColor(frame,frame,frame);
    m_background.setColorAt(0, m_backgroundColor);
    m_palette.setBrush(QPalette::Window, QBrush(m_background));
    setPalette(m_palette);
}

void KNMusicPlaylistListEditor::showAddMenu()
{
    m_addMenu->move(mapToGlobal(m_add->pos()));
    m_addMenu->exec();
}

void KNMusicPlaylistListEditor::createAddMenu()
{
    m_addMenu=new KNMenu(this);
    //Create all actions.
    for(int i=0; i<CategoryCount; i++)
    {
        m_addAction[i]=new QAction(m_addActionCaption[i],
                                   this);
        m_addMenu->addAction(m_addAction[i]);
    }
    connect(m_addAction[Playlist], SIGNAL(triggered()),
            this, SIGNAL(requireAddPlaylist()));
}
