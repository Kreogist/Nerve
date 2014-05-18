#include "knmusicheaderwidget.h"

KNMusicHeaderWidget::KNMusicHeaderWidget(QWidget *parent) :
    KNStdLibHeaderWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal=this->palette();
    pal.setColor(QPalette::Window, QColor(255,0,0));
    setPalette(pal);
}
