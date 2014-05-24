#ifndef KNMUSICDETAILINFO_H
#define KNMUSICDETAILINFO_H

#include <QScopedPointer>
#include <QStandardItemModel>
#include <QTreeView>

#include <QDialog>

class QPlainTextEdit;
class QTreeView;
class QLabel;
class KNLibMediaInfoParser;
class KNStdLibViewer;
class KNMusicTagEditor;
class KNMusicDetailOverview : public QWidget
{
    Q_OBJECT
public:
    enum MusicBasic
    {
        Name,
        Duration,
        Artist,
        Album,
        MusicBasicCount
    };

    enum MusicDetails
    {
        Kind,
        Size,
        DateModified,
        LastPlayed,
        Format,
        BitRate,
        SampleRate,
        Channels,
        CompressionMode,
        WritingLibrary,
        MusicDetailCount
    };
    explicit KNMusicDetailOverview(QWidget *parent = 0);
    void setText(const int &index, const QString &text);
    void setName(const QString &name);
    void setArtist(const QString &artist);
    void setAlbum(const QString &album);
    void setAlbumArt(const QPixmap &pixmap);
    void setBasicInfo(const int &index, const QString &basicData);
    void reset();

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    QLabel *m_labels[MusicDetailCount], *m_datas[MusicDetailCount],
           *m_basicInfo[MusicBasicCount], *m_albumArt;
    QString m_caption[MusicDetailCount];
};

class KNMusicDetailInfo : public QDialog
{
    Q_OBJECT
public:
    explicit KNMusicDetailInfo(QWidget *parent = 0);
    void setFilePath(const QString &filePath);

signals:

public slots:

private:
    KNMusicTagEditor *m_tagEditor;
    KNMusicDetailOverview *m_overall;
    KNLibMediaInfoParser *m_parser;
    KNStdLibViewer *m_switcher;
    QScopedPointer<QStandardItemModel> m_detailModel;
    QPlainTextEdit *m_detailText;
    QTreeView *m_detailViewer;
};

#endif // KNMUSICDETAILINFO_H
