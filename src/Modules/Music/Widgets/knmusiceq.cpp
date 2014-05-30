#include <QSlider>
#include <QLabel>
#include <QSignalMapper>
#include <QBoxLayout>
#include <QLineEdit>

#include "../../Public/bass/knlibbass.h"

#include "knmusiceq.h"

KNMusicEQ::KNMusicEQ(KNLibBass *backend, QWidget *parent) :
    QWidget(parent)
{
    //Backup the KNLibBass pointer.
    m_bassBackend=backend;

    //EQ layout.
    QBoxLayout *layout=new QBoxLayout(QBoxLayout::LeftToRight,
                                      this);
    layout->setSpacing(0);
    setLayout(layout);

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
                                                layout->widget());
        sliderLayout->setContentsMargins(0,0,0,0);
        sliderLayout->setSpacing(0);
        m_equalizerTune[i]=new QSlider(this);
        m_equalizerTune[i]->setRange(-40, 40);
        m_equalizerTune[i]->setValue(0);
        m_equalizerTune[i]->setTickPosition(QSlider::TicksBothSides);
        connect(m_equalizerTune[i], SIGNAL(valueChanged(int)),
                m_tuneRelease, SLOT(map()));
        m_tuneRelease->setMapping(m_equalizerTune[i], i);
        sliderLayout->addWidget(m_equalizerTune[i], 0, Qt::AlignHCenter);
        m_equalizerCaption[i]=new QLabel(m_bassBackend->eqFrequencyTitle(i),
                                           this);
        m_equalizerCaption[i]->setAlignment(Qt::AlignCenter);
        sliderLayout->addWidget(m_equalizerCaption[i]);
        QBoxLayout *editLayout=new QBoxLayout(QBoxLayout::LeftToRight,
                                                layout->widget());
        editLayout->setAlignment(Qt::AlignCenter);
        editLayout->setContentsMargins(0,0,0,0);
        editLayout->setSpacing(5);
        m_equalizerEdit[i]=new QLineEdit("0", this);
        m_equalizerEdit[i]->setAlignment(Qt::AlignRight);
        m_equalizerEdit[i]->setFixedWidth(30);
        connect(m_equalizerEdit[i], SIGNAL(textChanged(QString)),
                m_textChange, SLOT(map()));
        m_textChange->setMapping(m_equalizerEdit[i], i);
        editLayout->addWidget(m_equalizerEdit[i], 1, Qt::AlignRight);
        QLabel *editUnit=new QLabel("db", this);
        editLayout->addWidget(editUnit, 1);
        sliderLayout->addLayout(editLayout, 0);
        layout->addLayout(sliderLayout);
    }
}

void KNMusicEQ::setEQParam(const int &index)
{
    m_bassBackend->setEqualizerParam(index, m_equalizerTune[index]->value());
    m_equalizerEdit[index]->setText(QString::number(m_equalizerTune[index]->value()));
}

void KNMusicEQ::setTuneValue(const int &index)
{
    m_equalizerTune[index]->setValue(m_equalizerEdit[index]->text().toInt());
}
