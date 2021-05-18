#include "webui.h"
#include "model.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <string>


namespace webUi {

const std::string METHOD_POST = "POST";
const std::string METHOD_DELETE = "DELETE";
const std::string METHOD_GET = "GET";
const std::string METHOD_PUT = "PUT";

// private methods only used in this file
void handleNotFoundMethod(struct mg_connection *c) ;
void handleAnimesGet(struct mg_connection *c);
void handleAnimeGet(struct mg_connection *c, std::string id);
void handleAnimeDelete(struct mg_connection *c, std::string id);

void handleAnimePut(struct mg_http_message *hm, struct mg_connection *c, std::string id);
void handleAnimePost(struct mg_http_message *hm, struct mg_connection *c, std::string id);
void handleEpisodesGet(struct mg_http_message *hm, struct mg_connection *c);

void handleEpisodeGet(struct mg_connection *c, std::string id);
void handleEpisodePost(struct mg_http_message *hm, struct mg_connection *c, std::string id);
void handleEpisodePut(struct mg_http_message *hm, struct mg_connection *c, std::string id);
void handleEpisodeDelete(struct mg_connection *c, std::string id);


// member
/*
QSqlDatabase secondDB ;
database::AnimeRepository *animeRepository = nullptr;
database::EpisodeRepository *episodeRepository = nullptr ;
service::SearchService *searchService = nullptr;
*/
//new SearchService (episodeRepository,anime);



/*
bool openConnection()
{
    //QSqlDatabase secondDB = QSqlDatabase::database("QSQLITE", "second");

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
    searchService = new service::SearchService(episodeRepository);
    return true;
}
*/

// Stringifies the config. A caller must free() it.
char *stringify_config(struct config *cfg) {
    char *s = NULL;
    mjson_printf(mjson_print_dynamic_buf, &s, "{%Q:%d,%Q:%Q}", "value1",
                 cfg->value1, "value2", cfg->value2);
    return s;
}

// Update config structure. Return true if changed, false otherwise
bool update_config(struct mg_http_message *hm, struct config *cfg) {
    bool changed = false;
    char buf[256];
    double dv;
    if (mjson_get_number(hm->body.ptr, hm->body.len, "$.value1", &dv)) {
        s_config.value1 = dv;
        changed = true;
    }
    if (mjson_get_string(hm->body.ptr, hm->body.len, "$.value2", buf,
                         sizeof(buf)) > 0) {
        free(s_config.value2);
        s_config.value2 = strdup(buf);
        changed = true;
    }
    return changed;
}

// Notify all config watchers about the config change
void notify_config_change(struct mg_mgr *mgr) {
    struct mg_connection *c;
    char *s = stringify_config(&s_config);
    for (c = mgr->conns; c != NULL; c = c->next) {
        if (c->label[0] == 'W') mg_http_printf_chunk(c, "%s\n", s);
    }
    free(s);
}

// https://www.sitepoint.com/rest-api/
// HTTP request handler function. It implements the following endpoints:
//   /api/config/get - returns current config
//   /api/config/set - updates current config
//   /api/config/watch - does not return. Sends config as it changes in
//   chunks all other URI - serves web_root/ directory
void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        struct mg_mgr *mg =(struct mg_mgr *) fn_data;
        std::string method (hm->method.ptr,hm->method.len);
        QString Mymethod = QString::fromStdString(method);
        //qDebug() << "method " << Mymethod;
        char buf[255];
        int len;
        len = mg_url_decode(hm->uri.ptr, hm->uri.len, buf, sizeof(buf), 1);
        std::string newDecodeString (buf, len);
        QString decodeUrl = QString::fromStdString(newDecodeString);
        //qDebug() << "decode url is " << decodeUrl ;
        if (mg_http_match_uri(hm, "/api/anime/")) {
            //qDebug() << "url match /api/anime/ ";
            //get list of animes
            if(method == METHOD_GET) {
                handleAnimesGet(c);
            } else {
                handleNotFoundMethod(c);
            }
        } else if  (mg_http_match_uri(hm, "/api/anime/?*")) {
            //qDebug() << "url match /api/anime/?* ";

            int firstIndex = std::string("/api/anime/").size();
            char buf[255];
            int len;
            len = mg_url_decode(hm->uri.ptr, hm->uri.len, buf, sizeof(buf), 1);
            std::string decodeUrl (buf, len);
            std::string id = decodeUrl.substr(firstIndex);

            // if post method creates a user with the ID from url using the body data
            if (method == METHOD_POST && len > -1) {
                handleAnimePost(hm, c, id);
            } else if (method == METHOD_PUT  && len > -1) {
                // a PUT request to /api/anime/id updates user id with the body data
                handleAnimePut(hm, c, id);
            } else  if(method == METHOD_GET  && len > -1) {
                //a GET request to /api/anime/id returns the details of user id
                handleAnimeGet(c, id);
            } else if (method == METHOD_DELETE  && len > -1) {
                //a DELETE request to /api/anime/id deletes user id
                handleAnimeDelete(c, id);
            } else {
                handleNotFoundMethod(c);
            }
        } else if (mg_http_match_uri(hm, "/api/anime/?*/episode/")) {
            //qDebug() << "url match /api/anime/?*/episode/";
            if(method == METHOD_GET) {
                handleEpisodesGet(hm,c);
            }  else {
                handleNotFoundMethod(c);
            }
        } else if (mg_http_match_uri(hm, "/api/episode/?*")) {
            //qDebug() << "url match /api/episode/?*";

            int firstIndex = std::string("/api/episode/").size();
            char buf[255];
            int len;
            len = mg_url_decode(hm->uri.ptr, hm->uri.len, buf, sizeof(buf), 1);
            std::string decodeUrl (buf, len);
            std::string id = decodeUrl.substr(firstIndex);

            if(method == METHOD_GET && len > -1) {
                handleEpisodeGet(c,id);
            } else if (method == METHOD_POST && len > -1 ) {
                handleEpisodePost(hm, c,id);
            } else if (method == METHOD_DELETE && len > -1) {
                handleEpisodeDelete(c, id);
            } else if (method == METHOD_PUT && len > -1) {
                handleEpisodePut(hm, c,id);
            } else {
                handleNotFoundMethod(c);
            }
        }
        else if (mg_http_match_uri(hm, "/api/config/set")) {
            if (update_config(hm, &s_config)) notify_config_change(mg);
            mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
        } else if (mg_http_match_uri(hm, "/api/config/watch")) {
            c->label[0] = 'W';  // Mark ourselves as a config watcher
            mg_printf(c, "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
        } else {
            struct mg_http_serve_opts opts = {.root_dir = "web_root"};
            mg_http_serve_dir(c, hm, &opts);
        }
    }
}

void startWebApplication(service::SearchService *ser,
                         database::AnimeRepository *animeRep, database::EpisodeRepository *episodeRep) {
    searchService = ser;
    animeRepository = animeRep;
    episodeRepository = episodeRep;

    //struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    // listen on port 9000 and any available network binding (NULL == 0.0.0.0)
    mg_http_listen(&mgr, "http://0.0.0.0:9000", cb, &mgr);
    qDebug() << "web server application is started" ;
    for (;;) mg_mgr_poll(&mgr, 1000);
    //call it in the desctructer
    //mg_mgr_free(&mgr);
}

void shutDownWebApplication()
{
    mg_mgr_free(&mgr);
    delete episodeRepository;
    episodeRepository = nullptr;
    delete animeRepository;
    animeRepository = nullptr;
    delete searchService;
    searchService = nullptr;
}

void handleAnimeGet(struct mg_connection *c, std::string id)
{
    bool isFound = false;
    auto anime = animeRepository->findById(QString::fromStdString(id), &isFound);
    if(isFound ) {
        QJsonDocument doc;
        QJsonObject json;

        anime.write(json);
        doc.setObject(json);

        QByteArray jsonResponse = doc.toJson();
        char *s = new char[jsonResponse.size() + 1];
        strcpy(s, jsonResponse.constData());

        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\n",
                  jsonResponse.size() + 1, s);
        delete [] s;
    } else {
        handleNotFoundMethod(c);
    }
}

void handleAnimesGet(struct mg_connection *c)
{
    auto animes = animeRepository->findAll();
    QJsonArray animesArray;
    for(const auto &anime : animes) {
        QJsonObject torrentObject;
        anime.write(torrentObject);
        animesArray.append(torrentObject);
    }
    QJsonDocument doc;
    doc.setArray(animesArray);

    QByteArray jsonResponse = doc.toJson();
    char *s = new char[jsonResponse.size() + 1];
    strcpy(s, jsonResponse.constData());

    mg_printf(c, "HTTP/1.1 200 OK\rAccess-Control-Allow-Origin: *\r\nContent-Length: %d\r\n\r\n%s\n",
              jsonResponse.size() + 1, s);
    delete [] s;
}

void handleAnimePost(struct mg_http_message *hm, struct mg_connection *c, std::string id)
{
    //qDebug() << "handleAnimePost is called" ;
    const QByteArray json(hm->body.ptr, hm->body.len);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    //qDebug() << jsonDoc ;
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        //qDebug() << jsonDoc ;
        model::Anime anime;
        anime.read(jsonDoc.object());
        anime.name = QString::fromStdString(id);
        //qDebug() << anime.name << anime.searchKey;
        bool ok = animeRepository->add(anime);
        if(ok) {
            searchService->fetchEpisodeAnime(anime);
            mg_printf(c, "HTTP/1.1 201 Created\rAccess-Control-Allow-Origin: *\r\nContent-Length: 0\r\n\r\n");
        } else {
            mg_printf(c, "HTTP/1.1 400 Bad Request\rAccess-Control-Allow-Origin: *\r\nContent-Length: 0\r\n\r\n");
        }
    } else {
        mg_printf(c, "HTTP/1.1 400 Bad Request\rAccess-Control-Allow-Origin: *\r\nContent-Length: 0\r\n\r\n");
    }
}

void handleNotFoundMethod(struct mg_connection *c) {
    char s[] = "Not found";
    mg_printf(c, "HTTP/1.1 404 OK\r\nContent-Length: %d\r\n\r\n%s\n",
              (int) strlen(s) + 1, s);
}

void handleAnimePut(struct mg_http_message *hm, struct mg_connection *c, std::string id)
{
    //qDebug() << "handleAnimePut is called" ;
    const QByteArray json(hm->body.ptr, hm->body.len);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        //qDebug() << jsonDoc ;
        model::Anime anime;
        anime.read(jsonDoc.object());
        anime.name = QString::fromStdString(id);
        //qDebug() << anime.name << anime.searchKey;
        bool ok = animeRepository->update(anime);
        if(ok) {
            mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
        } else {
            mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
        }
    } else {
        mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
}

void handleEpisodesGet(struct mg_http_message *hm, struct mg_connection *c) {
    //qDebug() << "handleEpisodesGet is called" ;
    char buf[255];
    int len;
    len = mg_url_decode(hm->uri.ptr, hm->uri.len, buf, sizeof(buf), 1);
    if(len != -1) {
        std::string newDecodeString (buf, len);

        int firstIndex = std::string("/api/anime/").size();
        int numberOfChar = len - firstIndex - std::string("/episode/").size();
        std::string searchKey = newDecodeString.substr(firstIndex, numberOfChar);

        QString id = QString::fromStdString(searchKey);
        qDebug() << "search key is : " << id;

        if(!id.isEmpty()) {
            bool isFound = false;
            auto anime = animeRepository->findById(id, &isFound);
            qDebug() << "anime is founded "<< isFound;
            if(isFound) {
                auto episodes = episodeRepository->findEpisodesByAnime(anime);
                qDebug() << "episodes size is " << episodes.size();
                QJsonArray animesArray;
                for(const auto &anime : episodes) {
                    QJsonObject torrentObject;
                    anime.write(torrentObject);
                    animesArray.append(torrentObject);
                }
                QJsonDocument doc;
                doc.setArray(animesArray);

                //https://doc.qt.io/qt-5/qbytearray.html#data
                QByteArray jsonResponse = doc.toJson();
                char *s = new char[jsonResponse.size() + 1];
                strcpy(s, jsonResponse.constData());

                mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\n",
                          jsonResponse.size() + 1, s);
                delete [] s;
            } else {
                handleNotFoundMethod(c);
            }
        }
    }
    else {
        mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
}

void handleAnimeDelete(struct mg_connection *c, std::string id)
{
    //qDebug() << "handleAnimeDelete is called" ;
    QString newId = QString::fromStdString(id);
    model::Anime anime;
    anime.name = newId;
    bool ok = animeRepository->remove(anime);
    if(ok) {
        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    } else {
        mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
}

void handleEpisodePut(struct mg_http_message *hm, struct mg_connection *c, std::string id)
{
    //qDebug() << "handleEpisodePut is called" ;
    const QByteArray json(hm->body.ptr, hm->body.len);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        //qDebug() << jsonDoc ;
        model::Episode episode;
        episode.read(jsonDoc.object());
        episode.fileUrl= QString::fromStdString(id);
        bool ok = episodeRepository->update(episode);
        if(ok) {
            mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
        } else {
            mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
        }
    } else {
        mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
}

void handleEpisodePost(struct mg_http_message *hm, struct mg_connection *c, std::string id)
{
    //qDebug() << "handleEpisodePost is called" ;
    const QByteArray json(hm->body.ptr, hm->body.len);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        //qDebug() << jsonDoc ;
        model::Episode episode;
        episode.read(jsonDoc.object());
        episode.fileUrl = QString::fromStdString(id);
        bool ok = episodeRepository->add(episode);
        if(ok) {
            mg_printf(c, "HTTP/1.1 201 Created\r\nContent-Length: 0\r\n\r\n");
        } else {
            mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
        }
    } else {
        //qDebug() << "jsonDoc bad request" ;
        mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
}

void handleEpisodeDelete(struct mg_connection *c, std::string id)
{
    //qDebug() << "handleEpisodeDelete is called" ;
    QString newId = QString::fromStdString(id);
    model::Episode episode;
    episode.fileUrl = newId;
    bool ok = episodeRepository->remove(episode);
    if(ok) {
        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n");
    } else {
        mg_printf(c, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n");
    }
}

void handleEpisodeGet(struct mg_connection *c, std::string id)
{
    bool isFound = false;
    auto episode = episodeRepository->findById(QString::fromStdString(id), &isFound);
    if(isFound ) {
        QJsonDocument doc;
        QJsonObject json;

        episode.write(json);
        doc.setObject(json);

        QByteArray jsonResponse = doc.toJson();
        char *s = new char[jsonResponse.size() + 1];
        strcpy(s, jsonResponse.constData());

        mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\n",
                  jsonResponse.size() + 1, s);
        delete [] s;
    } else {
        handleNotFoundMethod(c);
    }
}

}
