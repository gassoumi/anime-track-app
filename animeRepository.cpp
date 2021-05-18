#include "animeRepository.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

database::AnimeRepositorySql::AnimeRepositorySql(QSqlDatabase db):m_db(db)
{

}

database::AnimeRepositorySql::~AnimeRepositorySql()
{

}

bool database::AnimeRepositorySql::add(const model::Anime &entity)
{
    // if one of those condition is true , we don't insert this torrent
    if( entity.name.isEmpty()) {
        qWarning() << "Impossible to add anime with empty name";
        return false;
    }


    QSqlQuery query(m_db);
    query.prepare("INSERT INTO anime (name, searchKey) "
                  "VALUES (?, ?)");
    query.addBindValue(entity.name);
    query.addBindValue(entity.searchKey);
    bool ok = query.exec();
    qDebug() << query.lastError();
    return ok;
}

bool database::AnimeRepositorySql::remove(const model::Anime &entity)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM anime WHERE name = ?");
    query.addBindValue(entity.name);
    bool ok = query.exec();
    qDebug() << query.lastError();
    return ok;
}

bool database::AnimeRepositorySql::update(const model::Anime &entity)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE anime SET searchKey = ? WHERE name = ?");
    query.addBindValue(entity.searchKey);
    query.addBindValue(entity.name);
    bool ok = query.exec();
    qDebug() << query.lastError();
    return ok;
}

QVector<model::Anime> database::AnimeRepositorySql::findAll()
{
    QVector<model::Anime> animes ;
    QSqlQuery query(m_db);
    query.exec("SELECT * FROM anime");
    while (query.next()) {
        model::Anime t;
        t.name = query.value(0).toString();
        t.searchKey = query.value(1).toString();
        t.dateAjout = query.value(2).toDateTime();
        animes.append(t);
    }
    return animes;
}

model::Anime database::AnimeRepositorySql::findById(const QString &id, bool *isFounded)
{
    QSqlQuery query(m_db);

    query.prepare("SELECT * FROM anime WHERE name = ?");
    query.addBindValue(id);
    query.exec();

    model::Anime anime;
    if(query.next())
    {
        anime.name = query.value(0).toString();
        anime.searchKey = query.value(1).toString();
        anime.dateAjout = query.value(2).toDateTime();
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
