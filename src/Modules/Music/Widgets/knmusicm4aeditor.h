#ifndef KNMUSICM4AEDITOR_H
#define KNMUSICM4AEDITOR_H

#include <QScopedPointer>
#include <QStandardItemModel>

#include "knmusictageditorbase.h"

class QTreeView;
class QComboBox;
class QPlainTextEdit;
class QLineEdit;
class KNVerticalWidgetSwitcher;
class KNMusicTagM4A;
class KNMusicM4AEditor : public KNMusicTagEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicM4AEditor(QWidget *parent = 0);
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
    enum ID3v2Items
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
        ID3v2ItemsCount
    };
    enum ID3v2CaptionItems
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
        ID3v2CaptionItemsCount
    };
    KNMusicTagM4A *m_tagM4A;
    KNVerticalWidgetSwitcher *m_switcher;
    QString m_caption[ID3v2CaptionItemsCount];
    QLabel *m_label[ID3v2CaptionItemsCount];
    QLineEdit *m_textEditor[9];
    QComboBox *m_genreList;
    QPlainTextEdit *m_commentEditor;
    QTreeView *m_advancedView;
    QScopedPointer<QStandardItemModel> m_advancedModel;
    QAction *m_toOverview, *m_toAdvanced;

};

#endif // KNMUSICM4AEDITOR_H
