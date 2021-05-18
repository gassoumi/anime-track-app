#include "webserver.h"
#include "webui.h"
#include <QDebug>
#include "animeRepository.h"
#include "episoderepository.h"
#include <QSqlDatabase>

WebServer::WebServer(QObject *parent) : QThread(parent)
{

}


WebServer::~WebServer()
{
    //mg_mgr_free(&mgr);
    webUi::shutDownWebApplication();
}

bool WebServer::openConnection()
{
    QSqlDatabase secondDB = QSqlDatabase::addDatabase("QSQLITE", "second");
    secondDB.setDatabaseName("anime.db");
    if (!secondDB.open()) {
        qWarning() << "Unable to establish a database connection.\n"
                   "This example needs SQLite support. Please read "
                   "the Qt SQL driver documentation for information how "
                   "to build it.\n\n"
                   "Click Cancel to exit." ;

        return false;
    }
    animeRepository = new database::AnimeRepositorySql(secondDB) ;
    episodeRepository = new database::EpisodeRepositorySql(secondDB) ;
    searchService = new service::SearchService();
    return true;
}

void WebServer::run() {
    if( openConnection()) {
        webUi::startWebApplication(searchService, animeRepository, episodeRepository);
    }
}


