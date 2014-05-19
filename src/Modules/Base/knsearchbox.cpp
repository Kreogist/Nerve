#include <QBoxLayout>
#include <QLineEdit>

#include <QDebug>

#include "knsearchbox.h"

KNSearchBox::KNSearchBox(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    setContentsMargins(0,0,0,0);

    m_palette=palette();
    m_palette.setColor(QPalette::Base, QColor(255,255,255));
    setPalette(m_palette);

    m_layout=new QBoxLayout(QBoxLayout::LeftToRight, this);
    m_layout->setContentsMargins(0,0,0,0);
    setLayout(m_layout);

    m_keyText=new QLineEdit(this);
    m_layout->addWidget(m_keyText);
}

void KNSearchBox::setPlaceHolderText(const QString &text)
{
    m_keyText->setPlaceholderText(text);
}
