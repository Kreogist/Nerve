#include <QBoxLayout>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QToolButton>
#include <QTimeLine>

#include <QDebug>

#include "../../Public/bass/knlibbass.h"

#include "../Libraries/knmusicmodel.h"
#include "../knmusicglobal.h"
#include "../../Base/knplayerprogress.h"

#include "knmusicdetailtooltip.h"

KNMusicDetailTooltipPlay::KNMusicDetailTooltipPlay(QWidget *parent) :
    QLabel(parent)
{
    setFixedSize(20,20);
    setScaledContents(true);
    m_play=QPixmap(":/Music/Resources/Music/preivew/play.png");
    m_pause=QPixmap(":/Music/Resources/Music/preivew/pause.png");
    setPixmap(m_play);
}

void KNMusicDetailTooltipPlay::reset()
{
    setPixmap(m_play);
    m_isButtonPlay=true;
}

void KNMusicDetailTooltipPlay::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
    if(m_isButtonPlay)
    {
        emit requirePlay();
        m_isButtonPlay=false;
        setPixmap(m_pause);
    }
    else
    {
        emit requirePause();
        m_isButtonPlay=true;
        setPixmap(m_play);
    }
}

KNMusicDetailTooltip::KNMusicDetailTooltip(QWidget *parent) :
    QWidget(parent)
{
    m_palette=palette();
    m_background=QColor(0x28,0x28,0x28);
    m_palette.setColor(QPalette::Window, m_background);
    m_palette.setColor(QPalette::WindowText, QColor(0x5f, 0x5f, 0x5f));
    setPalette(m_palette);

    setWindowFlags(Qt::ToolTip);
    m_tooltipDisapper=new QTimer(this);
    m_tooltipDisapper->setSingleShot(true);
    m_tooltipDisapper->setInterval(1500);
    connect(m_tooltipDisapper, SIGNAL(timeout()),
            this, SLOT(onActionHide()));

    //m_preview=new KNLibQtAV;
//    m_preview->moveToThread(&m_playerThread);

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

    QBoxLayout *previewPlayer=new QBoxLayout(QBoxLayout::LeftToRight,
                                             m_mainLayout->widget());
    previewPlayer->setContentsMargins(0,0,0,0);
    m_control=new KNMusicDetailTooltipPlay(this);
    previewPlayer->addWidget(m_control);
//    connect(m_control, SIGNAL(requirePlay()),
//            m_preview, SLOT(play()));
    connect(m_control, &KNMusicDetailTooltipPlay::requirePlay,
            this, &KNMusicDetailTooltip::requireHalfVolume);
//    connect(m_control, &KNMusicDetailTooltipPlay::requirePause,
//            m_preview, &KNLibQtAV::pause);
    connect(m_control, &KNMusicDetailTooltipPlay::requirePlay,
            this, &KNMusicDetailTooltip::requireRestoreHalfVolume);
    m_playerStatus=new KNPlayerProgress(this);
//    connect(m_preview, &KNLibQtAV::positionChanged,
//            this, &KNMusicDetailTooltip::onActionPositionChanged);
//    connect(m_preview, &KNLibQtAV::stopped,
//            this, &KNMusicDetailTooltip::requireRestoreHalfVolume);
    /*connect(m_playerStatus, &QSlider::sliderReleased,
            this, &KNMusicDetailTooltip::onActionSliderReleased);
    connect(m_playerStatus, &QSlider::sliderPressed,
            this, &KNMusicDetailTooltip::onActionSliderPressed);*/
    m_playerStatus->setMinimumWidth(400);
    previewPlayer->addWidget(m_playerStatus, 1);
    m_mainLayout->addLayout(previewPlayer);

    m_mouseIn=new QTimeLine(200, this);
    m_mouseIn->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseIn->setUpdateInterval(5);
    m_mouseIn->setEndFrame(0x50);
    connect(m_mouseIn, &QTimeLine::frameChanged,
            this, &KNMusicDetailTooltip::onActionChangeBackground);
    m_mouseOut=new QTimeLine(200, this);
    m_mouseOut->setEasingCurve(QEasingCurve::OutCubic);
    m_mouseOut->setUpdateInterval(5);
    m_mouseOut->setEndFrame(0x28);
    connect(m_mouseOut, &QTimeLine::frameChanged,
            this, &KNMusicDetailTooltip::onActionChangeBackground);

//    m_playerThread.start();
}

KNMusicDetailTooltip::~KNMusicDetailTooltip()
{
//    m_playerThread.quit();
//    m_playerThread.wait();
//    m_preview->deleteLater();
}

void KNMusicDetailTooltip::setTooltip(const QModelIndex &index,
                                      const QPoint &point)
{
    m_mouseOut->stop();
    m_mouseIn->stop();
    m_control->reset();
    onActionChangeBackground(0x28);
    m_playerStatus->setValue(0);
    m_tooltipDisapper->setInterval(1500);
    m_sliderPressed=false;
    if(m_currentRow==index.row())
    {
        return;
    }
    m_currentRow=index.row();
    m_albumArt->setPixmap(QPixmap::fromImage(m_musicModel->artwork(m_currentRow)));

    m_labels[Title]->setText(m_musicModel->itemText(m_currentRow, KNMusicGlobal::Name));
    m_labels[Time]->setText(m_musicModel->itemText(m_currentRow, KNMusicGlobal::Time));
    m_labels[Artist]->setText(m_musicModel->itemText(m_currentRow, KNMusicGlobal::Artist));

    m_filePath=m_musicModel->filePathFromIndex(m_currentRow);

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

void KNMusicDetailTooltip::forceQuit()
{
//    m_preview->stop();
    m_tooltipDisapper->stop();
//    disconnect(m_preview, &KNLibQtAV::stopped,
//               this, &KNMusicDetailTooltip::requireRestoreHalfVolume);
    hide();
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
    m_tooltipDisapper->setInterval(300);
    m_mouseOut->stop();
    m_mouseIn->setStartFrame(m_background.red());
    m_mouseIn->start();
//    m_preview->stop();
//    m_preview->setFilePath(m_filePath);
//    m_playerStatus->setMaximum(m_preview->duration());
}

void KNMusicDetailTooltip::leaveEvent(QEvent *event)
{
    m_mouseIn->stop();
    m_mouseOut->setStartFrame(m_background.red());
    m_mouseOut->start();
    QWidget::leaveEvent(event);
    m_tooltipDisapper->start();
}

void KNMusicDetailTooltip::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
//    m_preview->stop();
}

void KNMusicDetailTooltip::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)
    {
        hide();
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}

void KNMusicDetailTooltip::onActionHide()
{
    hide();
}

void KNMusicDetailTooltip::onActionPositionChanged(qint64 test)
{
    if(m_sliderPressed)
    {
        return;
    }
    m_playerStatus->setValue(test);
}

void KNMusicDetailTooltip::onActionSliderReleased()
{
//    bool playerPaused=m_preview->isPause();
//    m_preview->pause();
//    m_preview->setPosition(m_playerStatus->value());
//    if(!playerPaused)
//    {
//        m_preview->play();
//    }
}

void KNMusicDetailTooltip::onActionSliderPressed()
{
    m_sliderPressed=true;
}

void KNMusicDetailTooltip::onActionChangeBackground(const int &colorParam)
{
    m_background=QColor(colorParam, colorParam, colorParam);
    m_palette.setColor(QPalette::Window, m_background);
    int textParam=0x5f+(colorParam<<1);
    m_palette.setColor(QPalette::WindowText, QColor(textParam,textParam,textParam));
    setPalette(m_palette);
}

QPoint KNMusicDetailTooltip::bestPosition(const QPoint &pos)
{
    int tweakX=pos.x()+14, tweakY=pos.y()-60,
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
