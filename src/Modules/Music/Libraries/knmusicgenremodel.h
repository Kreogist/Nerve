#ifndef KNMUSICGENREMODEL_H
#define KNMUSICGENREMODEL_H

#include "knmusiccategorymodel.h"

class KNMusicGlobal;
class KNMusicGenreModel : public KNMusicCategoryModel
{
    Q_OBJECT
public:
    explicit KNMusicGenreModel(QObject *parent = 0);

signals:

public slots:
    void retranslate();
    void retranslateAndSet();

protected:
    QIcon itemIcon(const int &index) const;
    QString categoryName(const int &index) const;

private:
    KNMusicGlobal *m_musicGlobal;
};

#endif // KNMUSICGENREMODEL_H
