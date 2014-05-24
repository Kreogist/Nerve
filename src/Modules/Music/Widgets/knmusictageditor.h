#ifndef KNMUSICTAGEDITOR_H
#define KNMUSICTAGEDITOR_H

#include <QFile>
#include <QDataStream>
#include <QWidget>
#include <QPixmap>

class KNMusicTagEditorBase;
class KNMusicID3v1Editor;
class KNMusicID3v2Editor;
class KNMusicAPEv2Editor;
class KNMusicWMAEditor;
class KNMusicM4AEditor;
class KNMusicTagWAV;
class KNMusicTagFLAC;
class KNMusicTagEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicTagEditor(QWidget *parent = 0);
    void parseFile(const QString &filePath);
    QString title() const;
    QString album() const;
    QString artist() const;
    QPixmap albumArt() const;

signals:

public slots:

private:
    void resetEditor();
    void readBasicInfoFromEditor(KNMusicTagEditorBase *editor);
    void readFLACTag(QFile &mediaFile,
                     QDataStream &mediaData);
    void readWAVTag(QFile &mediaFile,
                    QDataStream &mediaData);
    KNMusicID3v1Editor *m_ID3v1Editor;
    KNMusicID3v2Editor *m_ID3v2Editor;
    KNMusicAPEv2Editor *m_APEv2Editor;
    KNMusicWMAEditor *m_WMAEditor;
    KNMusicM4AEditor *m_M4AEditor;
    KNMusicTagFLAC *m_tagFLAC;
    KNMusicTagWAV *m_tagWAV;

    enum BasicInformation
    {
        Title,
        Artist,
        Album,
        BasicInformationCount
    };

    QString m_basicInfo[BasicInformationCount];
    QPixmap m_coverArt;
};

#endif // KNMUSICTAGEDITOR_H
