#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QVector>
#include "model.h"

//https://docs.microsoft.com/en-us/cpp/cpp/abstract-classes-cpp?view=msvc-160
//https://www.geeksforgeeks.org/pure-virtual-destructor-c/
namespace database {

template <typename T, typename S>
class Repository
{
public:
    // =0 means abstract method(pure virtual)
    virtual bool add(const T &entity) = 0;
    virtual bool remove(const T &entity) = 0;
    virtual bool update(const T &entity) = 0;
    virtual QVector<T> findAll() = 0;
    virtual T findById(const S &id, bool *isFounded = nullptr) = 0;
    virtual ~Repository() = 0;

};

class EpisodeRepository : public Repository<model::Episode,QString>
{
public:
    // without constructor
    /*
     *     // see open classroom course to check if we write the below functions
    virtual bool add(const model::Episode &entity) = 0;
    virtual bool remove(const model::Episode &entity) = 0;
    virtual bool update(const model::Episode &entity) = 0;
    virtual QVector<model::Episode> findAll() = 0;
    virtual model::Episode findById(const QString &id, bool *isFounded = nullptr) = 0;
    */
    virtual QVector<model::Episode> findEpisodesByAnime(const model::Anime &anime) = 0;
    virtual model::Episode findByNumber(const model::Episode &episode, bool *isFounded = nullptr) =0 ;
    virtual ~EpisodeRepository() {} ;
};

class AnimeRepository : public Repository<model::Anime,QString>
{
    // see open classroom course to check if we write the below functions

public:
    virtual ~AnimeRepository() {} ;
};

}


template <typename T, typename S>
database::Repository<T,S>::~Repository() {}


#endif // REPOSITORY_H
