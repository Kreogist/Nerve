
#include <QLabel>

#include <QDebug>

#include "knlibviewercategorybutton.h"

KNLibViewerCategoryButton::KNLibViewerCategoryButton(QWidget *parent) :
    QWidget(parent)
{
    ;
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
