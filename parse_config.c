
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "goagent-client.h"
#include "ini.h"

static struct option longopts[] = {
    {"inifile", required_argument, NULL, 'f'},
    {"host", required_argument, NULL, 'H'},
    {"port", required_argument, NULL, 'p'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    { NULL, 0, NULL, 0 }
};

profile* find_profile(const char* name, configuration* config, int create)
{
    profile* p = config->profiles;
    while (p && strcmp(name,p->name)) {
        p = p->next;
    }
    if (!p && create) {
        p = malloc(sizeof(profile));
        memset(p,0,sizeof(profile));
        p->name = strdup(name);
        profile* head = config->profiles;
        if (!head) {
            config->profiles=p;
        } else {
            while (head->next) {
                head = head->next;
            }
            head->next = p;
        }
    }
    return p;
}

int ini_file_handler (void* pconfig, const char* section,
        const char* key, const char* value)
{
#define MATCH_SECTION(s) strcmp(section, s) == 0
#define MATCH_KEY(s) strcmp(key,s) == 0
    configuration* config = (configuration*)pconfig;
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
    } else if (strstr(section,"google_") == section) {
        profile* p = find_profile(section,config,1);
        if (MATCH_KEY("mode")) {
            p->mode = strdup(value);
        } else if (MATCH_KEY("window")) {
            p->window = atoi(value);
        } else if (MATCH_KEY("hosts")) {
            p->hosts = strdup(value);
        } else if (MATCH_KEY("sites")) {
            p->sites = strdup(value);
        } else if (MATCH_KEY("forcehttps")) {
            p->forcehttps = strdup(value);
        } else if (MATCH_KEY("withgae")) {
            p->withgae = strdup(value);
        } else if (MATCH_KEY("withdns")) {
            p->withdns = strdup(value);
        } else {
            handled = 0;
        }
    }
    if (!handled) {
        fprintf(stderr,"Unrecognized section:%s key:%s\r\n", section, key);
    }
    return 1;
}

int switch_profile(configuration *config, const char* name)
{
    profile* p = find_profile(name, config, 0);
    if (!p) {
       return 1;
    }
    config->current_profile = p;
    return 0;
}

static void usage() {
    printf("=================GoAgent native client================\r\n");
    printf("Version 0.1.0\r\n\r\n");
    printf("-f FILE, --file=FILE      Read configuration from FILE\r\n");
    printf("-H, --host                Change listening ip\r\n");
    printf("-P, --port                Change listening port\r\n");
    printf("-h, --help                Print this message and exit\r\n");
}

int getoption(int argc, char **argv, configuration *config) {
    int ch;
    int loaded_config = 0;
    while ((ch = getopt_long(argc, argv, "f:h:p:", longopts, NULL)) != -1) {
        if (!loaded_config) {
            char *file;
            if (ch == 'f') {
                file = optarg;
            } else {
                file = "./proxy.ini";
            }
            if (ini_parse(file, ini_file_handler, config) < 0) {
                fprintf(stderr, "Can't load %s\r\n",file);
                return 1;
            } else {
                loaded_config = 1;
            }
        }
        switch (ch) {
            case 'f':
                break;
            case 'H':
                config->listen_ip = optarg;
                break;
            case 'p':
                config->listen_port = atoi(optarg);
                break;
            case 'V':
            case 'h':
            default:
                usage();
                return 1;
        }
    }
    if (!loaded_config) {
        if (ini_parse("proxy.ini", ini_file_handler, config) < 0) {
            fprintf(stderr, "Can't load proxy.ini\r\n");
            return 1;
        }
    }
    if (switch_profile(config, config->gae_profile)) {
        fprintf(stderr, "Can't find gae profile:%s\r\n",config->gae_profile);
        return 1;
    }
    return 0;
}
