#ifndef EPISODEREPOSITORY_H
#define EPISODEREPOSITORY_H

#include "repository.h"
#include "model.h"
#include <QVector>
#include <QString>
#include <QSqlDatabase>

namespace database {
class EpisodeRepositorySql : public EpisodeRepository
{
public:
    EpisodeRepositorySql(QSqlDatabase db);
    virtual bool add(const model::Episode &entity) override;
    virtual bool remove(const model::Episode &entity) override;
    virtual bool update(const model::Episode &entity) override;
    virtual QVector<model::Episode> findAll() override;
    virtual model::Episode findById(const QString &id, bool *isFounded = nullptr) override;
    virtual model::Episode findByNumber(const model::Episode &episode, bool *isFounded = nullptr) override;
    virtual QVector<model::Episode> findEpisodesByAnime(const model::Anime &anime) override;
    virtual ~EpisodeRepositorySql();
private:
    QSqlDatabase m_db;
};
}


#endif // EPISODEREPOSITORY_H
