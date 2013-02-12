
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>
#include <getopt.h>

#include "goagent-client.h"
#include "ini.h"

int ini_file_handler (void* pconfig, const char* section, 
                      const char* key, const char* value)
{
    proxy_configuration* config = (proxy_configuration*)pconfig;    
    if (strcmp(section,"listen") == 0) {
        if (strcmp(key,"ip") == 0) {
            config->listen_ip = strdup(value);        
        } else if (strcmp(key,"port") == 0) {
            config->listen_port = atoi(value);
        }
        return 1;
    }
    return 1;
}

int main(int argc, char **argv)
{
    proxy_configuration config;
    if (ini_parse("proxy.ini", ini_file_handler, &config) < 0) {
        printf("Can't load 'proxy.ini'\r\n");
        return 1;
    }
    printf("test\r\nlisten:%s:%d\r\n",config.listen_ip,config.listen_port);
    return 0;
}
