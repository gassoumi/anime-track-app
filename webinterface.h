#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "mongoose.h"
#ifdef __cplusplus
}
#endif

#include "animeRepository.h"
#include "episoderepository.h"
#include "repository.h"
#include "model.h"
#include <QJsonDocument>

namespace web {

class WebInterface
{
public:
    WebInterface();
    ~WebInterface();

private:
    void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
    void startWebUi() ;

private:
    database::AnimeRepository *m_animeRep;
    database::EpisodeRepository *m_episodeRep;
};

}


#endif // WEBINTERFACE_H
