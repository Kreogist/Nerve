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
    m_tooltipDisapper->setInterval(1500);
    connect(m_tooltipDisapper, SIGNAL(timeout()),
            this, SLOT(onActionHide()));

    m_mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                this);
    m_mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
    setLayout(m_mainLayout);
    QBoxLayout *albumLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                           m_mainLayout->widget());
    m_mainLayout->addLayout(albumLayout);
    m_albumArt=new QLabel(this);
    m_albumArt->setFixedSize(128, 128);
    m_albumArt->setScaledContents(true);
    albumLayout->addWidget(m_albumArt);

    QBoxLayout *labelLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                           m_mainLayout->widget());
    albumLayout->addLayout(labelLayout);
    labelLayout->setSizeConstraint(QLayout::SetMinimumSize);
    for(int i=0; i<ToolTipItemsCount; i++)
    {
        m_labels[i]=new QLabel(this);
        m_labels[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        labelLayout->addWidget(m_labels[i]);
    }
    QFont nameFont=m_labels[Title]->font();
    nameFont.setBold(true);
    m_labels[Title]->setFont(nameFont);
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

    resize(m_mainLayout->sizeHint());
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
    m_tooltipDisapper->start();
    QWidget::leaveEvent(event);
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
