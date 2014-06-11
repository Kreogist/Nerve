#include <QBoxLayout>
#include <QLabel>

#include "knmusicloop.h"

KNMusicLoopButton::KNMusicLoopButton(QWidget *parent) :
    QLabel(parent)
{
    ;
}

void KNMusicLoopButton::mousePressEvent(QMouseEvent *event)
{
    QLabel::mousePressEvent(event);
    m_isPressed=true;
}

void KNMusicLoopButton::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    if(m_isPressed)
    {
        m_isPressed=false;
        emit clicked();
    }
}

KNMusicLoop::KNMusicLoop(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(16);
    //Set the main layout.
    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    m_loopTypeImage[KNMusicGlobal::NoRepeat]=
            QPixmap(":/Music/Resources/Music/mode/NoRepeat.png");
    m_loopTypeImage[KNMusicGlobal::RepeatAll]=
            QPixmap(":/Music/Resources/Music/mode/Repeat.png");
    m_loopTypeImage[KNMusicGlobal::RepeatSong]=
            QPixmap(":/Music/Resources/Music/mode/RepeatSingle.png");

    m_loopType=new KNMusicLoopButton(this);
    m_loopType->setScaledContents(true);
    m_loopType->setFixedSize(16,16);
    m_loopType->setPixmap(m_loopTypeImage[KNMusicGlobal::NoRepeat]);
    connect(m_loopType, &KNMusicLoopButton::clicked,
            this, &KNMusicLoop::onActionChangeType);
    m_mainLayout->addWidget(m_loopType);
}

void KNMusicLoop::onActionChangeType()
{
    m_loopMode=m_loopMode==KNMusicGlobal::LoopCount-1?0:m_loopMode+1;
    m_loopType->setPixmap(m_loopTypeImage[m_loopMode]);
    emit requireChangeLoopType(m_loopMode);
}
