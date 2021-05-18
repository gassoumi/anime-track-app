

#ifdef __cplusplus
extern "C"
{
#endif
#include "webui.h"

#ifdef __cplusplus
}
#endif

#include "animeRepository.h"
#include "repository.h"
#include "model.h"

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

// HTTP request handler function. It implements the following endpoints:
//   /api/config/get - returns current config
//   /api/config/set - updates current config
//   /api/config/watch - does not return. Sends config as it changes in
//   chunks all other URI - serves web_root/ directory
void cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        struct mg_mgr *mg =(struct mg_mgr *) fn_data;
        if (mg_http_match_uri(hm, "/api/config/get")) {
            char *s = stringify_config(&s_config);
            mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\n",
                      (int) strlen(s) + 1, s);
            free(s);
        } else if (mg_http_match_uri(hm, "/api/animes")) {
            char *s = stringify_config(&s_config);
            mg_printf(c, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\n",
                      (int) strlen(s) + 1, s);
            free(s);
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

void startWebUi() {
    struct mg_mgr mgr;
    mg_mgr_init(&mgr);
    mg_http_listen(&mgr, "http://localhost:9000", cb, &mgr);
    for (;;) mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
}
