
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "goagent-client.h"

int ini_file_handler (void* pconfig, const char* section,
                      const char* key, const char* value)
{
    #define MATCH_SECTION(s) strcmp(section, s) == 0
    #define MATCH_KEY(s) strcmp(key,s) == 0
    proxy_configuration* config = (proxy_configuration*)pconfig;
    int handled = 1;
    if (MATCH_SECTION("listen")) {
        if (MATCH_KEY("ip")) {
            config->listen_ip = strdup(value);
        } else if (MATCH_KEY("port")) {
            config->listen_port = atoi(value);
        } else if (MATCH_KEY("visible")) {
            config->listen_visible = atoi(value);
        } else if (MATCH_KEY("debuginfo")) {
            config->listen_debuginfo = atoi(value);
        } else {
            handled = 0;
        }
    } else if (MATCH_SECTION("gae")) {
        if (MATCH_KEY("appid")) {
            config->gae_appid = strdup(value);
        } else if (MATCH_KEY("password")) {
            config->gae_password = strdup(value);
        } else if (MATCH_KEY("path")) {
            config->gae_path = strdup(value);
        } else if (MATCH_KEY("profile")) {
            config->gae_profile = strdup(value);
        } else if (MATCH_KEY("crlf")) {
            config->gae_crlf = atoi(value);
        } else if (MATCH_KEY("validate")) {
            config->gae_validate = atoi(value);
        } else {
            handled = 0;
        }
    } else if (MATCH_SECTION(config->gae_profile)) {
        if (MATCH_KEY("mode")) {
            config->profile_mode = strdup(value);
        } else if (MATCH_KEY("window")) {
            config->profile_window = atoi(value);
        } else if (MATCH_KEY("hosts")) {
            config->profile_hosts = strdup(value);
        } else if (MATCH_KEY("sites")) {
            config->profile_sites = strdup(value);
        } else if (MATCH_KEY("forcehttps")) {
            config->profile_forcehttps = strdup(value);
        } else if (MATCH_KEY("withgae")) {
            config->profile_withgae = strdup(value);
        } else if (MATCH_KEY("withdns")) {
            config->profile_withdns = strdup(value);
        } else {
            handled = 0;
        }
    }
    if (!handled) {
        fprintf(stderr,"Unrecognized section:%s key:%s\r\n", section, key);
    }
    return 1;
}
