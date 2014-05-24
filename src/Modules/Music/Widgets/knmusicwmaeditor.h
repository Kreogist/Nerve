#ifndef KNMUSICWMAEDITOR_H
#define KNMUSICWMAEDITOR_H

#include <QScopedPointer>
#include <QStandardItemModel>

#include "knmusictageditorbase.h"

class QLineEdit;
class QTreeView;
class KNVerticalWidgetSwitcher;
class KNMusicTagWMA;
class KNMusicWMAEditor : public KNMusicTagEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicWMAEditor(QWidget *parent = 0);
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

    void setEditorText(const int &index, const QString &data);

private:
    enum WMACaptionItems
    {
        Name,
        Artist,
        Album,
        Year,
        Track,
        Genre,
        Comment,
        WMACaptionItemsCount
    };
    QString m_caption[WMACaptionItemsCount];
    QLabel *m_label[WMACaptionItemsCount];
    QLineEdit *m_textEditor[WMACaptionItemsCount];
    KNMusicTagWMA *m_tagWMA;
    QTreeView *m_advancedView;
    QScopedPointer<QStandardItemModel> m_advancedModel;
    KNVerticalWidgetSwitcher *m_switcher;
    QAction *m_toOverview, *m_toAdvanced;
};

#endif // KNMUSICWMAEDITOR_H
