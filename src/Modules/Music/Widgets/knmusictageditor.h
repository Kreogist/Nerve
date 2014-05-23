#ifndef KNMUSICTAGEDITOR_H
#define KNMUSICTAGEDITOR_H

#include <QFile>
#include <QDataStream>
#include <QWidget>

class KNMusicTagEditorBase;
class KNMusicID3v1Editor;
class KNMusicID3v2Editor;
class KNMusicTagAPEv2;
class KNMusicTagWMA;
class KNMusicTagM4A;
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

signals:

public slots:

private:
    void readBasicInfoFromEditor(KNMusicTagEditorBase *editor);
    void readAPEv2Tag(QFile &mediaFile,
                      QDataStream &mediaData);
    void readWMATag(QFile &mediaFile,
                    QDataStream &mediaData);
    void readM4ATag(QFile &mediaFile,
                    QDataStream &mediaData);
    void readFLACTag(QFile &mediaFile,
                     QDataStream &mediaData);
    void readWAVTag(QFile &mediaFile,
                    QDataStream &mediaData);
    KNMusicID3v1Editor *m_ID3v1Editor;
    KNMusicID3v2Editor *m_ID3v2Editor;
    KNMusicTagAPEv2 *m_tagAPEv2;
    KNMusicTagWMA *m_tagWMA;
    KNMusicTagM4A *m_tagM4A;
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
};

#endif // KNMUSICTAGEDITOR_H
