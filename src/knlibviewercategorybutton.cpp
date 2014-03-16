
#include <QLabel>

#include <QDebug>

#include "knlibviewercategorybutton.h"

KNLibViewerCategoryButton::KNLibViewerCategoryButton(QWidget *parent) :
    QWidget(parent)
{
    //Set basic labels.
    m_icon=new QLabel(this);

    m_caption=new QLabel(this);
}

void KNLibViewerCategoryButton::setIcon(const QPixmap &value)
{
    m_icon->setPixmap(value);
}

void KNLibViewerCategoryButton::setText(const QString &value)
{
    m_caption->setText(value);
}

bool KNLibViewerCategoryButton::isChecked() const
{
    return m_checked;
}

void KNLibViewerCategoryButton::setChecked(bool checked)
{
    m_checked = checked;
    emit toggled(checked);
    onCheckedChanged();
}
