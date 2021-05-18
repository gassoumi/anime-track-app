#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include <QThread>
#include "repository.h"
#include "searchservice.h"

class WebServer : public  QThread
{
    Q_OBJECT
public:
    explicit WebServer(QObject *parent = nullptr);
    ~WebServer();
    void run() override;

private :
    bool openConnection();

private:
    //struct mg_mgr mgr;
    database::AnimeRepository *animeRepository = nullptr;
    database::EpisodeRepository *episodeRepository = nullptr ;
    service::SearchService *searchService = nullptr;
};

#endif // WEBSERVER_H
