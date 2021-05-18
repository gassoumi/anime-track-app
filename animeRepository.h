#ifndef ANIMEREPOSITORY_H
#define ANIMEREPOSITORY_H

#include "model.h"
#include "repository.h"
#include <QSqlDatabase>

namespace database {

class AnimeRepositorySql : public AnimeRepository
{
public:
    AnimeRepositorySql(QSqlDatabase db);
    virtual bool add(const model::Anime &entity) override;
    virtual bool remove(const model::Anime &entity) override;
    virtual bool update(const model::Anime &entity) override;
    virtual QVector<model::Anime> findAll() override;
    virtual model::Anime findById(const QString &id, bool *isFounded = nullptr) override;
    virtual ~AnimeRepositorySql();
private:
    QSqlDatabase m_db;
};

}


#endif // ANIMEREPOSITORY_H
