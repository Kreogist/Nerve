#ifndef KNMUSICAPEV2EDITOR_H
#define KNMUSICAPEV2EDITOR_H

#include <QScopedPointer>
#include <QStandardItemModel>

#include "knmusictageditorbase.h"

class QLineEdit;
class QTreeView;
class KNVerticalWidgetSwitcher;
class KNMusicTagAPEv2;
class KNMusicAPEv2Editor : public KNMusicTagEditorBase
{
    Q_OBJECT
public:
    explicit KNMusicAPEv2Editor(QWidget *parent = 0);
    void readTag(QFile &mediaFile,
                 QDataStream &mediaData);
    void resetEditor();
    QString title() const;
    QString album() const;
    QString artist() const;

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

private slots:
    void toAdvancedMode();
    void toOverviewMode();

private:
    void setEditorText(const int &index, const QString &data);
    enum APEv2CaptionItems
    {
        Name,
        Artist,
        Album,
        Year,
        Track,
        Genre,
        Comment,
        APEv2CaptionItemsCount
    };
    QString m_caption[APEv2CaptionItemsCount];
    QLabel *m_label[APEv2CaptionItemsCount];
    QLineEdit *m_textEditor[APEv2CaptionItemsCount];
    KNMusicTagAPEv2 *m_tagAPEv2;
    QTreeView *m_advancedView;
    QScopedPointer<QStandardItemModel> m_advancedModel;
    KNVerticalWidgetSwitcher *m_switcher;
    QAction *m_toOverview, *m_toAdvanced;
};

#endif // KNMUSICAPEV2EDITOR_H
