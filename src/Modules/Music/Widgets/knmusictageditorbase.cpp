#include "knmusictageditorbase.h"

KNMusicTagEditorBase::KNMusicTagEditorBase(QWidget *parent) :
    QWidget(parent)
{
}

void KNMusicTagEditorBase::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    Q_UNUSED(mediaFile);
    Q_UNUSED(mediaData);
}

void KNMusicTagEditorBase::resetEditor()
{
    ;
}

QString KNMusicTagEditorBase::title() const
{
    return QString();
}

QString KNMusicTagEditorBase::album() const
{
    return QString();
}

QString KNMusicTagEditorBase::artist() const
{
    return QString();
}

QPixmap KNMusicTagEditorBase::albumArt() const
{
    return QPixmap();
}
