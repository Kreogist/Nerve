#ifndef KNMUSICVIEWERMENUBASE_H
#define KNMUSICVIEWERMENUBASE_H

#include "../../../Modules/Base/knmenu.h"

class QStandardItemModel;
class KNMusicViewerMenuBase : public KNMenu
{
    Q_OBJECT
public:
    explicit KNMusicViewerMenuBase(QWidget *parent = 0);
    virtual void setModel(QStandardItemModel *model)=0;

signals:
    void requirePlayMusic(const QString &filePath);
    void requireShowIn(int category,
                       const QModelIndex &index);
    void requireGetInfo(const QString &filePath);
    void requireDelete(const QModelIndex &index);
    void requireDeleteSelection();

public slots:

};

#endif // KNMUSICVIEWERMENUBASE_H
