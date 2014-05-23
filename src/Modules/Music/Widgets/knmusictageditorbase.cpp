#include "knmusictageditorbase.h"

KNMusicTagEditorBase::KNMusicTagEditorBase(QWidget *parent) :
    QWidget(parent)
{
}

void KNMusicTagEditorBase::readTag(QFile &mediaFile, QDataStream &mediaData)
{
    ;
}

void KNMusicTagEditorBase::resetEditor()
{
    ;
}

KNMusicTagBase *KNMusicTagEditorBase::musicTagReader()
{
    return NULL;
}
