#ifndef KNMUSICARTISTMODEL_H
#define KNMUSICARTISTMODEL_H

#include "knmusiccategorymodel.h"

class KNMusicArtistModel : public KNMusicCategoryModel
{
    Q_OBJECT
public:
    explicit KNMusicArtistModel(QObject *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    void updateImage(const int &index);
    QIcon itemIcon(const int &index) const;
    QString categoryName(const int &index) const;

private:
};

#endif // KNMUSICARTISTMODEL_H
