#include <QLabel>
#include <QSignalMapper>
#include <QBoxLayout>

#include "../../Public/bass/knlibbass.h"
#include "../../Base/knlabeleditor.h"
#include "knmusiceqslider.h"

#include "knmusiceq.h"

KNMusicEQ::KNMusicEQ(KNLibBass *backend, QWidget *parent) :
    QWidget(parent)
{
    //Backup the KNLibBass pointer.
    m_bassBackend=backend;

    QBoxLayout *mainLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                          this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    QBoxLayout *controlLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                             mainLayout->widget());
    controlLayout->setContentsMargins(0,0,0,0);
    controlLayout->setSpacing(0);
    mainLayout->addLayout(controlLayout);

    //EQ layout.
    QBoxLayout *sliderList=new QBoxLayout(QBoxLayout::LeftToRight,
                                          mainLayout->widget());
    sliderList->setContentsMargins(0,0,0,0);
    sliderList->setSpacing(0);

    //Add the tuner signal mapper for 9 tuners.
    m_tuneRelease=new QSignalMapper(this);
    connect(m_tuneRelease, SIGNAL(mapped(int)),
            this, SLOT(setEQParam(int)));
    m_textChange=new QSignalMapper(this);
    connect(m_textChange, SIGNAL(mapped(int)),
            this, SLOT(setTuneValue(int)));
    //Initial the 31 step tunner.
    for(int i=0; i<KNLibBass::EqualizerCount; i++)
    {
        QBoxLayout *sliderLayout=new QBoxLayout(QBoxLayout::TopToBottom,
                                                sliderList->widget());
        sliderLayout->setContentsMargins(0,0,0,0);
        sliderLayout->setSpacing(0);
        m_equalizerCaption[i]=new QLabel(m_bassBackend->eqFrequencyTitle(i),
                                           this);
        m_equalizerCaption[i]->setAlignment(Qt::AlignCenter);
        sliderLayout->addWidget(m_equalizerCaption[i]);
        m_equalizerTune[i]=new KNMusicEQSlider(this);
        m_equalizerTune[i]->setRange(-40, 40);
        m_equalizerTune[i]->setValue(0);
        connect(m_equalizerTune[i], SIGNAL(sliderMoved(float)),
                m_tuneRelease, SLOT(map()));
        m_tuneRelease->setMapping(m_equalizerTune[i], i);
        sliderLayout->addWidget(m_equalizerTune[i]);
        m_equalizerEdit[i]=new KNLabelEditor("0", this);
        m_equalizerEdit[i]->setAlignment(Qt::AlignCenter);
        //m_equalizerEdit[i]->setFixedWidth(30);
        connect(m_equalizerEdit[i], SIGNAL(textEdited(QString)),
                m_textChange, SLOT(map()));
        m_textChange->setMapping(m_equalizerEdit[i], i);
        sliderLayout->addWidget(m_equalizerEdit[i]);
        sliderList->addLayout(sliderLayout);
    }
    mainLayout->addLayout(sliderList);
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
