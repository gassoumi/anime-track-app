#include "episoderepository.h"
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QSqlError>

database::EpisodeRepositorySql::EpisodeRepositorySql(QSqlDatabase db):m_db(db)
{

}

database::EpisodeRepositorySql::~EpisodeRepositorySql()
{

}

bool database::EpisodeRepositorySql::add(const model::Episode &entity)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO episode (fileUrl, fileName,fileSize,nbSeeders,"
                  "nbLeechers,siteUrl,descrLink,number,name) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(entity.fileUrl);
    query.addBindValue(entity.fileName);
    query.addBindValue(entity.fileSize);
    query.addBindValue(entity.nbSeeders);
    query.addBindValue(entity.nbLeechers);
    query.addBindValue(entity.siteUrl);
    query.addBindValue(entity.descrLink);
    query.addBindValue(entity.number);
    query.addBindValue(entity.anime.name);
    bool ok = query.exec();
    qDebug() << "add Episode" << query.lastError();
    return ok;
}

model::Episode database::EpisodeRepositorySql::findByNumber(const model::Episode &episode, bool *isFounded)
{
    QSqlQuery query(m_db);
    // qDebug() << "id in findEpisode by number" ;
    query.prepare("SELECT *  From episode WHERE number = ? AND name = ?");
    query.addBindValue(episode.number);
    query.addBindValue(episode.anime.name);
    query.exec();

    model::Episode mEpisode;
    if(query.next())
    {
        mEpisode.fileUrl = query.value(0).toString();
        mEpisode.fileName = query.value(1).toString();
        mEpisode.fileSize = query.value(2).toDouble();
        mEpisode.nbSeeders = query.value(3).toDouble();
        mEpisode.nbLeechers = query.value(4).toDouble();
        mEpisode.siteUrl = query.value(5).toString();
        mEpisode.descrLink = query.value(6).toString();
        mEpisode.number =  query.value(7).toInt();
        if(isFounded != nullptr) {
            *isFounded = true;
        }
    } else {
        if(isFounded != nullptr) {
            *isFounded = false;
        }
    }
    return mEpisode;
}

bool database::EpisodeRepositorySql::remove(const model::Episode &entity)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM episode WHERE fileUrl = ?");
    query.addBindValue(entity.fileUrl);
    bool ok = query.exec();
    qDebug() << query.lastError();
    return ok;

}

bool database::EpisodeRepositorySql::update(const model::Episode &entity)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE episode SET fileName = ?, fileSize = ?, nbSeeders = ?, "
                  "nbLeechers = ?, siteUrl = ?, descrLink = ?, number = ?"
                  " WHERE fileUrl = ?");
    query.addBindValue(entity.fileName);
    query.addBindValue(entity.fileSize);
    query.addBindValue(entity.nbSeeders);
    query.addBindValue(entity.nbLeechers);
    query.addBindValue(entity.siteUrl);
    query.addBindValue(entity.descrLink);
    query.addBindValue(entity.number);
    query.addBindValue(entity.fileUrl);
    bool ok = query.exec();
    qDebug() << query.lastError();
    return ok;
}

QVector<model::Episode> database::EpisodeRepositorySql::findAll()
{
    QVector<model::Episode> episodes ;
    QSqlQuery query(m_db);
    query.exec("SELECT * FROM episode");
    while (query.next()) {
        model::Episode t;
        t.fileUrl = query.value(0).toString();
        t.fileName = query.value(1).toString();
        t.fileSize = query.value(2).toDouble();
        t.nbSeeders = query.value(3).toDouble();
        t.nbLeechers = query.value(4).toDouble();
        t.siteUrl = query.value(5).toString();
        t.descrLink = query.value(6).toString();
        t.number =  query.value(7).toInt();
        // todo get the anime from the other repository
        episodes.append(t);
    }
    return episodes;
}

model::Episode database::EpisodeRepositorySql::findById(const QString &id, bool *isFounded)
{
    QSqlQuery query(m_db);

    query.prepare("SELECT * episode WHERE fileUrl = ?");
    query.addBindValue(id);
    query.exec();

    model::Episode anime;
    if(query.next())
    {
        anime.fileUrl = query.value(0).toString();
        anime.fileName = query.value(1).toString();
        anime.fileSize = query.value(2).toDouble();
        anime.nbSeeders = query.value(3).toDouble();
        anime.nbLeechers = query.value(4).toDouble();
        anime.siteUrl = query.value(5).toString();
        anime.descrLink = query.value(6).toString();
        anime.number =  query.value(7).toInt();
        if(isFounded != nullptr) {
            *isFounded = true;
        }
    } else {
        if(isFounded != nullptr) {
            *isFounded = false;
        }
    }
    return anime;
}

QVector<model::Episode> database::EpisodeRepositorySql::findEpisodesByAnime(const model::Anime &anime)
{
    QVector<model::Episode> episodes ;
    QSqlQuery query(m_db);
    query.prepare("SELECT * FROM episode WHERE name = ?");
    query.addBindValue(anime.name);

    query.exec();
    qDebug() << query.lastError();

    while (query.next()) {
        model::Episode episode;
        episode.fileUrl = query.value(0).toString();
        episode.fileName = query.value(1).toString();
        episode.fileSize = query.value(2).toDouble();
        episode.nbSeeders = query.value(3).toDouble();
        episode.nbLeechers = query.value(4).toDouble();
        episode.siteUrl = query.value(5).toString();
        episode.descrLink = query.value(6).toString();
        episode.number =  query.value(7).toInt();
        // todo get the anime from the other repository
        episode.anime = anime;
        episodes.append(episode);
    }
    return episodes;
}
