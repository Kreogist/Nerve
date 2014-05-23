#ifndef KNMUSICTAGEDITORBASE_H
#define KNMUSICTAGEDITORBASE_H

#include <QFile>
#include <QDataStream>
#include <QLabel>

#include <QWidget>

#include "../Libraries/knmusictagbase.h"

class KNMusicTagEditorBase : public QWidget
{
    Q_OBJECT
public:
    explicit KNMusicTagEditorBase(QWidget *parent = 0);
    virtual void readTag(QFile &mediaFile,
                         QDataStream &mediaData);
    virtual void resetEditor();
    virtual KNMusicTagBase *musicTagReader();
    virtual QString title() const;
    virtual QString album() const;
    virtual QString artist() const;

signals:

public slots:

};

#endif // KNMUSICTAGEDITORBASE_H
