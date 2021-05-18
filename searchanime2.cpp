#include "searchanime2.h"
#include  "searchhandler.h"
#include "repository.h"
#include "episoderepository.h"
#include "utils.h"

namespace service {

int SearchAnime::nextIdDB = 0 ;

SearchAnime::SearchAnime(const model::Anime &anime)
    : QObject()
    , mAnime(anime)
{
    nextIdDB++;
    connectionName = "connection" + QString::number(nextIdDB);
    db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName("anime.db");
    if(db.open()) {
        isDbOpen = true;
    }

}

SearchAnime::~SearchAnime()
{
    qDebug() << tr(" SearchAnime destructor is called");
    db.close();
    db.removeDatabase(connectionName);
}

void SearchAnime::newSearchResultsRecevied(const QVector<model::Episode> &results)
{
    qDebug() << " newSearchResultsRecevied is called" ;
    QString msg = "hello world";
    // signal doesn't work fix this later
    emit resultReady(results, mAnime);

    if (isDbOpen) {
        database::EpisodeRepository *episodeRepository = new database::EpisodeRepositorySql(db) ;
        utils::handlenewSearchResults(results,episodeRepository,mAnime);
        delete episodeRepository;
    } else {
        qWarning() << "Unable to establish a database connection.\n"
                   "This example needs SQLite support. Please read "
                   "the Qt SQL driver documentation for information how "
                   "to build it.\n\n"
                   "Click Cancel to exit." ;
        qDebug() << "can not open database in search anime class";
    }
    qDebug() << " newSearchResultsRecevied is finished" ;
}

void SearchAnime::searchFinishedRecevied(bool cancelled)
{
    QString message = cancelled ? tr("Search is finish but it was cancelled") : tr("Search is finish");
    qDebug() << message << " in search anime class " << " | " << QDateTime::currentDateTime() << "|" << mAnime.searchKey ;
}

void SearchAnime::doWork()
{
    qDebug() << "do work is called";
    QStringList engines;
    engines << "all";
    qDebug() << mAnime.searchKey << "in search anime";
    SearchHandler *mSearchHandler = new SearchHandler(mAnime.searchKey, "anime",engines);
    connect(mSearchHandler,&SearchHandler::newSearchResults, this, &SearchAnime::newSearchResultsRecevied);
    connect(mSearchHandler,&SearchHandler::searchFinished, this, &SearchAnime::searchFinishedRecevied);
    connect(mSearchHandler,&SearchHandler::searchFailed, this, &SearchAnime::searchFailedRecevied);
    connect(this, &QObject::destroyed, mSearchHandler, &QObject::deleteLater);
}

void SearchAnime::searchFailedRecevied()
{
    qWarning() << tr("Search fail") ;
}


}
