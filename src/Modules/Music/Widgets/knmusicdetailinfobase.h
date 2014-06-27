#ifndef KNMUSICDETAILINFOBASE_H
#define KNMUSICDETAILINFOBASE_H

#include <QDialog>

class KNMusicDetailInfoBase : public QDialog
{
    Q_OBJECT
public:
    explicit KNMusicDetailInfoBase(QWidget *parent = 0);
    virtual void setFilePath(const QString &filePath)=0;

signals:

public slots:

};

#endif // KNMUSICDETAILINFOBASE_H
