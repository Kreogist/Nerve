#ifndef KNMUSICALBUMMODEL_H
#define KNMUSICALBUMMODEL_H

#include "knmusiccategorymodel.h"

class KNMusicAlbumModel : public KNMusicCategoryModel
{
    Q_OBJECT
public:
    explicit KNMusicAlbumModel(QObject *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    QIcon itemIcon(const int &index) const;
    QString categoryName(const int &index) const;

};

#endif // KNMUSICALBUMMODEL_H
