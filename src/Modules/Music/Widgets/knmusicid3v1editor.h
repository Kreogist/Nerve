#ifndef KNMUSICID3V1EDITOR_H
#define KNMUSICID3V1EDITOR_H

#include <QList>

#include "knmusictageditorbase.h"

class QComboBox;
class QLineEdit;
class QFormLayout;
class KNMusicTagID3v1;
class KNMusicID3v1Editor : public KNMusicTagEditorBase
{
    Q_OBJECT
public:
    enum ID3v1Items
    {
        Title,
        Artist,
        Album,
        Year,
        Comment,
        Track,
        Genre,
        ID3v1ItemsCount
    };
    explicit KNMusicID3v1Editor(QWidget *parent = 0);
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
    void writeTag(QFile &mediaFile,
                  QDataStream &mediaData);

private slots:
    void updateCount(const int i);

private:
    KNMusicTagID3v1 *m_tagID3v1;
    QFormLayout *m_mainLayout;
    QLabel *m_captionsLabel[ID3v1ItemsCount],
           *m_textCountLabel[Track+1], *m_textLimitLabel[Track+1], *m_willSetAs[Track+1];
    char m_realText[30];
    int m_textLimit[Track+1];
    QString m_captions[ID3v1ItemsCount];
    QString m_realSetText;
    QLineEdit *m_textEditor[6];
    QComboBox *m_genreCombo;
};

#endif // KNMUSICID3V1EDITOR_H
