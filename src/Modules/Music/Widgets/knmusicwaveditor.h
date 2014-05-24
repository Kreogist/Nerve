#ifndef KNMUSICWAVEDITOR_H
#define KNMUSICWAVEDITOR_H

#include <QScopedPointer>
#include <QStandardItemModel>

#include "knmusictageditorbase.h"

class QTreeView;
class QComboBox;
class QPlainTextEdit;
class QLineEdit;
class KNVerticalWidgetSwitcher;
class KNMusicTagWAV;
class KNMusicWAVEditor : public KNMusicTagEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicWAVEditor(QWidget *parent = 0);
    void readTag(QFile &mediaFile,
                 QDataStream &mediaData);
    void resetEditor();
    QString title() const;
    QString album() const;
    QString artist() const;
    QPixmap albumArt() const;

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void toAdvancedMode();
    void toOverviewMode();
private:
    void setEditorText(const int &index, const QString &text);
    enum WAVItems
    {
        Name,
        Artist,
        Album,
        AlbumArtist,
        TrackNumber,
        TrackCount,
        DiscNumber,
        DiscCount,
        Year,
        Comment,
        Genre,
        WAVItemsCount
    };
    enum WAVCaptionItems
    {
        CaptionName,
        CaptionArtist,
        CaptionAlbum,
        CaptionYear,
        CaptionAlbumArtist,
        CaptionTrack,
        CaptionDisc,
        CaptionGenre,
        CaptionComment,
        WAVCaptionItemsCount
    };
    KNMusicTagWAV *m_tagWAV;
    KNVerticalWidgetSwitcher *m_switcher;
    QString m_caption[WAVCaptionItemsCount];
    QLabel *m_label[WAVCaptionItemsCount];
    QLineEdit *m_textEditor[9];
    QComboBox *m_genreList;
    QPlainTextEdit *m_commentEditor;
    QTreeView *m_advancedView;
    QScopedPointer<QStandardItemModel> m_advancedModel;
    QAction *m_toOverview, *m_toAdvanced;
};

#endif // KNMUSICWAVEDITOR_H
