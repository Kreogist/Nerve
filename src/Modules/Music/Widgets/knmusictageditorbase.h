#ifndef KNMUSICTAGEDITORBASE_H
#define KNMUSICTAGEDITORBASE_H

#include <QFile>
#include <QDataStream>
#include <QLabel>

#include <QWidget>

class KNMusicTagEditorBase : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicTagEditorBase(QWidget *parent = 0);
    virtual void readTag(QFile &mediaFile,
                         QDataStream &mediaData);

signals:

public slots:

};

#endif // KNMUSICTAGEDITORBASE_H
