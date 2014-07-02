#include <QBoxLayout>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "knmusicviewcontainer.h"

KNMusicViewContainer::KNMusicViewContainer(QWidget *parent) :
    QWidget(parent)
{
    //Set properties.
    setAcceptDrops(true);
    setContentsMargins(0,0,0,0);

    //Set layout.
    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    m_layout->setSpacing(0);
    setLayout(m_layout);
}

void KNMusicViewContainer::setCentralWidget(QWidget *widget)
{
    m_layout->addWidget(widget);
}

void KNMusicViewContainer::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void KNMusicViewContainer::dropEvent(QDropEvent *event)
{
    emit requireAnalysisUrls(event->mimeData()->urls());
}
