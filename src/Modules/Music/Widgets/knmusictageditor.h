#ifndef KNMUSICTAGEDITOR_H
#define KNMUSICTAGEDITOR_H

#include <QFile>
#include <QDataStream>
#include <QWidget>

class KNMusicID3v1Editor;
class KNMusicTagID3v2;
class KNMusicTagAPEv2;
class KNMusicTagWma;
class KNMusicTagM4A;
class KNMusicTagWAV;
class KNMusicTagFLAC;
class KNMusicTagEditor : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicTagEditor(QWidget *parent = 0);
    void parseFile(const QString &filePath);

signals:

public slots:

private:
    void readID3v1Tag(QFile &mediaFile,
                      QDataStream &mediaData);
    void readID3v2Tag(QFile &mediaFile,
                      QDataStream &mediaData);
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
    KNMusicTagID3v2 *m_tagID3v2;
    KNMusicTagAPEv2 *m_tagAPEv2;
    KNMusicTagWma *m_tagWMA;
    KNMusicTagM4A *m_tagM4A;
    KNMusicTagFLAC *m_tagFLAC;
    KNMusicTagWAV *m_tagWAV;
};

#endif // KNMUSICTAGEDITOR_H
