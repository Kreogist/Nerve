#ifndef KNMUSICLISTVIEWHEADER_H
#define KNMUSICLISTVIEWHEADER_H

#include "../knmusicglobal.h"

#include <QHeaderView>

class KNMusicListViewHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit KNMusicListViewHeader(QWidget *parent = 0);
    void moveToFirst(int logicalIndex);

signals:

public slots:

};

#endif // KNMUSICLISTVIEWHEADER_H
