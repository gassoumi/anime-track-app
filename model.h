#ifndef EPISODE_H
#define EPISODE_H

#include <QString>
#include <QDateTime>
#include <QVector>
#include <QJsonObject>


namespace model  {
struct Episode ;
struct Anime ;
}

struct model::Anime {
    Anime();
    ~Anime();
    //Anime(const Anime &other);
    QString name;
    QString searchKey;
    QDateTime dateAjout;
    QVector<model::Episode> episodes;
    bool operator==(const model::Anime &other) const;
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

/*
QDebug operator<<(QDebug dbg, const model::Anime &anime)
{
    if (anime.name.isEmpty() && anime.searchKey.isEmpty())
        dbg.nospace() << "Anime()";
    else
        dbg.nospace() << "Anime(" << anime.name << "," << anime.searchKey <<")";
    return dbg.maybeSpace();
}
*/

inline bool operator==(const  model::Anime &e1, const  model::Anime &e2)
{
    return e1.name == e2.name;
}

inline uint qHash(const model::Anime &key, uint seed)
{
    return qHash(key.name, seed) ^ key.dateAjout.date().day() ;
}

struct model::Episode {
    Episode();
    //Episode(const Episode &other);
    ~Episode();

    QString fileName;
    QString fileUrl;
    qlonglong fileSize;
    qlonglong nbSeeders;
    qlonglong nbLeechers;
    QString siteUrl;
    QString descrLink;
    int number;
    model::Anime anime;
    bool operator==(const model::Episode &other) const;
    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
};

inline bool operator==(const  model::Episode &e1, const  model::Episode &e2)
{
    return e1.number == e2.number;
}

inline uint qHash(const model::Episode &key, uint seed)
{
    return qHash(key.number, seed)  ^ qHash(key.fileUrl, seed)  ;
}

typedef model::Episode SearchResult;
//typedef QMap<QString, QVector<SearchResult>> QMapTorrent;

Q_DECLARE_METATYPE(QVector<SearchResult>)
Q_DECLARE_METATYPE(model::Anime)
Q_DECLARE_METATYPE(model::Episode)
#endif // EPISODE_H
