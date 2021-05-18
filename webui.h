#ifndef WEBUI_H
#define WEBUI_H

#include "mjson.h"
#include <string>
#include "searchservice.h"
#include "animeRepository.h"
#include "episoderepository.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "mongoose.h"
#ifdef __cplusplus
}
#endif

//https://isocpp.org/wiki/faq/pointers-to-members#fnptr-vs-memfnptr-types

namespace webUi {

//https://stackoverflow.com/questions/1637332/static-const-vs-define/1637367
extern const std::string METHOD_POST;
extern const std::string METHOD_DELETE;
extern const std::string METHOD_GET;
extern const std::string METHOD_PUT;

static database::AnimeRepository *animeRepository = nullptr;
static database::EpisodeRepository *episodeRepository = nullptr ;
static service::SearchService *searchService = nullptr;

// This is a configuration structure we're going to show on a dashboard
static struct config {
    int value1;
    char *value2;
} s_config = {123, NULL};

static struct mg_mgr mgr;

// Stringifies the config. A caller must free() it.
char *stringify_config(struct config *cfg) ;

// Update config structure. Return true if changed, false otherwise
bool update_config(struct mg_http_message *hm, struct config *cfg) ;

// Notify all config watchers about the config change
void notify_config_change(struct mg_mgr *mgr) ;

// HTTP request handler function. It implements the following endpoints:
//   /api/config/get - returns current config
//   /api/config/set - updates current config
//   /api/config/watch - does not return. Sends config as it changes in
//   chunks all other URI - serves web_root/ directory
void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data);

void startWebApplication(service::SearchService *ser,
                         database::AnimeRepository *animeRep, database::EpisodeRepository *episodeRep) ;

void shutDownWebApplication();

//bool openConnection();

}


#endif // WEBUI_H
