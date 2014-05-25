#include <QBoxLayout>
#include <QTimer>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>

#include <QDebug>

#include "../Libraries/knmusicmodel.h"
#include "../knmusicglobal.h"

#include "knmusicdetailtooltip.h"

KNMusicDetailTooltip::KNMusicDetailTooltip(QWidget *parent) :
    QWidget(parent)
{
    setWindowFlags(Qt::ToolTip);
    m_tooltipDisapper=new QTimer(this);
    m_tooltipDisapper->setSingleShot(true);
    m_tooltipDisapper->setInterval(2000);
    connect(m_tooltipDisapper, SIGNAL(timeout()),
            this, SLOT(onActionHide()));

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    setLayout(mainLayout);
    QBoxLayout *albumLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           mainLayout->widget());
    mainLayout->addLayout(albumLayout);
    m_albumArt=new QLabel(this);
    m_albumArt->setFixedSize(128, 128);
    m_albumArt->setScaledContents(true);
    albumLayout->addWidget(m_albumArt);

    QBoxLayout *labelLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                           mainLayout->widget());
    albumLayout->addLayout(labelLayout);
    for(int i=0; i<ToolTipItemsCount; i++)
    {
        m_labels[i]=new QLabel(this);
        labelLayout->addWidget(m_labels[i]);
    }
    labelLayout->addStretch();
}

void KNMusicDetailTooltip::setTooltip(const QModelIndex &index,
                                      const QPoint &point)
{
    if(m_currentRow==index.row())
    {
        return;
    }

    m_currentRow=index.row();
    m_albumArt->setPixmap(QPixmap::fromImage(m_musicModel->artwork(m_currentRow)));

    m_labels[Title]->setText(m_musicModel->itemText(m_currentRow, KNMusicGlobal::Name));
    m_labels[Time]->setText(m_musicModel->itemText(m_currentRow, KNMusicGlobal::Time));
    m_labels[Artist]->setText(m_musicModel->itemText(m_currentRow, KNMusicGlobal::Artist));

    resize(sizeHint());
    move(bestPosition(point));
}

void KNMusicDetailTooltip::setMusicModel(KNMusicModel *model)
{
    m_musicModel=model;
}

void KNMusicDetailTooltip::showTooltip()
{
    if(!isVisible())
    {
        show();
    }
    m_tooltipDisapper->stop();
    m_tooltipDisapper->start();
}

void KNMusicDetailTooltip::retranslate()
{
    ;
}

void KNMusicDetailTooltip::retranslateAndSet()
{
    ;
}

void KNMusicDetailTooltip::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    m_tooltipDisapper->stop();
}

void KNMusicDetailTooltip::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    m_tooltipDisapper->start();
}

void KNMusicDetailTooltip::onActionHide()
{
    hide();
}

QPoint KNMusicDetailTooltip::bestPosition(const QPoint &pos)
{
    int tweakX=pos.x()+10, tweakY=pos.y(),
        screenHeight=QApplication::desktop()->height(),
        screenWidth=QApplication::desktop()->width();
    if(tweakX+width()>screenWidth)
    {
        tweakX=screenWidth-width()-5;
    }
    if(tweakY+height()>screenHeight)
    {
        tweakY=screenHeight-height();
    }
    return QPoint(tweakX, tweakY);
}
