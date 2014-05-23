#ifndef KNMUSICAPEV2EDITOR_H
#define KNMUSICAPEV2EDITOR_H

#include "knmusictageditorbase.h"

class KNMusicAPEv2Editor : public KNMusicTagEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicAPEv2Editor(QWidget *parent = 0);
    void readTag(QFile &mediaFile,
                 QDataStream &mediaData);
    void resetEditor();
    KNMusicTagBase *musicTagReader();
    QString title() const;
    QString album() const;
    QString artist() const;

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    enum APEv2CaptionItems
    {
        CaptionName,
        CaptionArtist,
        CaptionAlbum,
        CaptionYear,
        CaptionTrack,
        CaptionGenre,
        CaptionComment,
        APEv2CaptionItemsCount
    };
    QString m_caption[APEv2CaptionItemsCount];
    QLabel *m_label[APEv2CaptionItemsCount];
};

#endif // KNMUSICAPEV2EDITOR_H
