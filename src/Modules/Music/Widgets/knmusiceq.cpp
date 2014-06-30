#include <QLabel>
#include <QSignalMapper>
#include <QBoxLayout>

#include "../../Public/Base/knmusicbackend.h"
#include "../../Base/knlabeleditor.h"
#include "knmusicvolumeslider.h"
#include "knmusiceqslider.h"

#include "knmusiceq.h"

KNMusicEQ::KNMusicEQ(KNMusicBackend *backend, QWidget *parent) :
    QWidget(parent)
{
    retranslate();
    setAutoFillBackground(true);

    QPalette pal=palette();
    pal.setColor(QPalette::Window, QColor(0x20,0x20,0x20,200));
    pal.setColor(QPalette::WindowText, QColor(0xff,0xff,0xff));
    pal.setColor(QPalette::Text, QColor(0xff,0xff,0xff));
    pal.setColor(QPalette::ButtonText, QColor(0xff,0xff,0xff));
    setPalette(pal);

    //Backup the KNMusicBackend pointer.
    m_bassBackend=backend;

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    //EQ layout.
    QBoxLayout *sliderList=new QBoxLayout(QBoxLayout::LeftToRight,
                                          mainLayout->widget());
    sliderList->setContentsMargins(0,0,0,0);
    sliderList->setSpacing(0);
    //Add the control layout.
    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                             mainLayout->widget());
    m_rangeLabel=new QLabel(m_rangeCaption, this);
    m_rangeLabel->setAlignment(Qt::AlignCenter);
    controlLayout->addWidget(m_rangeLabel);
    m_rangeControl=new KNMusicEQSlider(this);
    m_rangeControl->setRange(1, 100);
    m_rangeControl->setValue(40);
    controlLayout->addWidget(m_rangeControl, 1);
    m_rangeEdit=new KNLabelEditor("40", this);
    m_rangeEdit->setAlignment(Qt::AlignCenter);
    connect(m_rangeControl, &KNMusicEQSlider::valueChanged,
            [=]
            {
                QString rawText=QString::number(m_rangeControl->value()).left(4);
                m_rangeEdit->setText("."==rawText.right(1)?
                                     rawText.left(rawText.length()-1):
                                     rawText);
            });
    connect(m_rangeEdit, &KNLabelEditor::textEdited,
            [=]
            {
                m_rangeControl->setValue(m_rangeEdit->text().toFloat());
            });
    controlLayout->addWidget(m_rangeEdit);
    sliderList->addLayout(controlLayout);
    sliderList->addStretch();
    //Add the tuner signal mapper for 9 tuners.
    m_tuneRelease=new QSignalMapper(this);
    connect(m_tuneRelease, SIGNAL(mapped(int)),
            this, SLOT(setEQParam(int)));
    m_textChange=new QSignalMapper(this);
    connect(m_textChange, SIGNAL(mapped(int)),
            this, SLOT(setTuneValue(int)));
    //Initial the 31 step tunner.
    for(int i=0; i<KNMusicBackend::EqualizerCount; i++)
    {
        QBoxLayout *sliderLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                sliderList->widget());
        sliderLayout->setContentsMargins(0,0,0,0);
        sliderLayout->setSpacing(0);
        m_equalizerCaption[i]=new QLabel(m_bassBackend->eqFrequencyTitle(i),
                                           this);
        m_equalizerCaption[i]->setMinimumWidth(30);
        m_equalizerCaption[i]->setAlignment(Qt::AlignCenter);
        sliderLayout->addWidget(m_equalizerCaption[i], 0, Qt::AlignHCenter);
        m_equalizerTune[i]=new KNMusicEQSlider(this);
        m_equalizerTune[i]->setRange(-40, 40);
        connect(m_rangeControl, &KNMusicVolumeSlider::valueChanged,
                [=](const float &sideValue)
                {
                    m_equalizerTune[i]->setRange(-sideValue, sideValue);
                });
        m_equalizerTune[i]->setValue(0);
        connect(m_equalizerTune[i], SIGNAL(valueChanged(float)),
                m_tuneRelease, SLOT(map()));
        m_tuneRelease->setMapping(m_equalizerTune[i], i);
        sliderLayout->addWidget(m_equalizerTune[i], 1, Qt::AlignHCenter);
        m_equalizerEdit[i]=new KNLabelEditor("0", this);
        m_equalizerEdit[i]->setAlignment(Qt::AlignCenter);
        //m_equalizerEdit[i]->setFixedWidth(30);
        connect(m_equalizerEdit[i], SIGNAL(textEdited(QString)),
                m_textChange, SLOT(map()));
        m_textChange->setMapping(m_equalizerEdit[i], i);
        sliderLayout->addWidget(m_equalizerEdit[i], 0, Qt::AlignHCenter);
        sliderList->addLayout(sliderLayout);
    }
    sliderList->addStretch();
    mainLayout->addLayout(sliderList);
}

void KNMusicEQ::retranslate()
{
    m_rangeCaption=tr("Range");
}

void KNMusicEQ::retranslateAndSet()
{
    retranslate();
}

void KNMusicEQ::setEQParam(const int &index)
{
    m_bassBackend->setEqualizerParam(index, m_equalizerTune[index]->value());
    QString rawText=QString::number(m_equalizerTune[index]->value()).left(4);
    m_equalizerEdit[index]->setText("."==rawText.right(1)?
                                        rawText.left(rawText.length()-1):
                                        rawText);
}

void KNMusicEQ::setTuneValue(const int &index)
{
    m_equalizerTune[index]->setValue(m_equalizerEdit[index]->text().toFloat());
    m_bassBackend->setEqualizerParam(index, m_equalizerTune[index]->value());
}
