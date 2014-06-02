#include <QBoxLayout>

#include "knmusicvisualeffect.h"

#include "knmusicplayerwidget.h"

KNMusicPlayerWidget::KNMusicPlayerWidget(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0,0,0,200));
    setPalette(pal);

    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom, this);
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);

    m_playListLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                    m_mainLayout->widget());
    m_playListLayout->setContentsMargins(0,0,0,0);
    m_playListLayout->setSpacing(0);
    m_mainLayout->addLayout(m_playListLayout);
//    m_visualEffect=new KNMusicVisualEffect;
//    m_visualEffect->resize(368, 70);
}
