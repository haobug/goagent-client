
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

#include "goagent-client.h"

int main(int argc, char **argv)
{
    printf("=================GoAgent native client================\r\n");
    printf("Version 0.1.0\r\n\r\n");
    configuration config;
    memset(&config,0,sizeof(configuration));
    if (getoption(argc,argv,&config)) {
        return 1;
    }
    printf("test\r\nlisten:%s:%d\r\ngae id:%s hosts:%s\r\n",config.listen_ip,config.listen_port,config.gae_appid,config.current_profile->hosts);
    return 0;
}
