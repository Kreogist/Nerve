#ifndef KNMUSICFLACEDITOR_H
#define KNMUSICFLACEDITOR_H

#include <QScopedPointer>
#include <QStandardItemModel>

#include "knmusictageditorbase.h"

class QTreeView;
class QComboBox;
class QPlainTextEdit;
class QLineEdit;
class KNVerticalWidgetSwitcher;
class KNMusicTagFLAC;
class KNMusicFLACEditor : public KNMusicTagEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicFLACEditor(QWidget *parent = 0);
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
    enum FLACItems
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
        FLACItemsCount
    };
    enum FLACCaptionItems
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
        FLACCaptionItemsCount
    };
    KNMusicTagFLAC *m_tagFLAC;
    KNVerticalWidgetSwitcher *m_switcher;
    QString m_caption[FLACCaptionItemsCount];
    QLabel *m_label[FLACCaptionItemsCount];
    QLineEdit *m_textEditor[9];
    QComboBox *m_genreList;
    QPlainTextEdit *m_commentEditor;
    QTreeView *m_advancedView;
    QScopedPointer<QStandardItemModel> m_advancedModel;
    QAction *m_toOverview, *m_toAdvanced;

};

#endif // KNMUSICFLACEDITOR_H
