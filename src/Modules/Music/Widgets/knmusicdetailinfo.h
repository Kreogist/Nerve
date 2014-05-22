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
    enum MusicDetails
    {
        Kind,
        Size,
        Duration,
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

public slots:
    void retranslate();
    void retranslateAndSet();

private:
    QLabel *m_labels[MusicDetailCount], *m_datas[MusicDetailCount];
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
