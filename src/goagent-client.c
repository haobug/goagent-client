
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
//#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <dirent.h>
#include <errno.h>
//#include <netinet/in.h>
//#include <sys/socket.h>
//#include <resolv.h>
//#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

#include "goagent-client.h"

int main(int argc, char **argv)
{
	struct event_base *base;
    struct evconnlistener *listener;
    struct event *signal_event;

    struct sockaddr_in sin;
#ifdef WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

	configuration config;
    printf("=================GoAgent native client================\r\n");
    printf("Version 0.1.0\r\n\r\n");
    
    memset(&config,0,sizeof(configuration));
    if (getoption(argc,argv,&config)) {
        return 1;
    }
    printf("test\r\nlisten:%s:%d\r\ngae id:%s hosts:%s\r\n",config.listen_ip,config.listen_port,config.gae_appid,config.current_profile->hosts);


    base = event_base_new();  
    return 0;
}
