#include "webinterface.h"

web::WebInterface::WebInterface()
{
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    // listen on port 9000 and any available network binding (NULL == 0.0.0.0)
    void (web::WebInterface::*func) (struct mg_connection *, int ev, void *ev_data, void *fn_data);

    func = &WebInterface::cb;
    typedef  int (web::WebInterface::*cb) (struct mg_connection *, int ev, void *ev_data, void *fn_data);
    typedef  void (WebInterface::*FredMemFn)(struct mg_connection *, int ev, void *ev_data, void *fn_data);
    FredMemFn p = &WebInterface::cb;
    mg_http_listen(&mgr, "http://0.0.0.0:9000", *p, &mgr);
    for (;;) mg_mgr_poll(&mgr, 1000);
}

void web::WebInterface::cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {

}

void  web::WebInterface::startWebUi() {

}
