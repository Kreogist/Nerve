#include <QBoxLayout>

#include "../../Base/knsearchbox.h"

#include "knmusicheaderwidget.h"

KNMusicHeaderWidget::KNMusicHeaderWidget(QWidget *parent) :
    KNStdLibHeaderWidget(parent)
{
    retranslate();

    m_mainLayout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    setLayout(m_mainLayout);

    m_searchBox=new KNSearchBox(this);
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
    connect(m_searchBox, &KNSearchBox::textChanged,
            this, &KNMusicHeaderWidget::requireSearch);
    m_mainLayout->addWidget(m_searchBox);
}

void KNMusicHeaderWidget::retranslate()
{
    m_searchPlaceHolder=tr("Search Music");
}

void KNMusicHeaderWidget::retranslateAndSet()
{
    retranslate();
    m_searchBox->setPlaceHolderText(m_searchPlaceHolder);
}
