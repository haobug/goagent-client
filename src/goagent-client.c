
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
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

static void read_cb(struct bufferevent *bev, void *ctx){
    struct evbuffer *input;
    char *request_line;
    size_t len;
    client_handler* clienthandler;
    char buf[1024];
    int n;
    
    clienthandler=(client_handler*)ctx;
    
    input = bufferevent_get_input(bev);
    while ((n = evbuffer_remove(input, buf, sizeof(buf))) > 0) {
        http_parser_execute(&clienthandler->req_parser, &clienthandler->parse_req_settings, buf, n);
    }
    
    request_line = evbuffer_readln(input, &len, EVBUFFER_EOL_CRLF);
    if (!request_line) {
        /* The first line has not arrived yet. */
        return;
    } else {
        fprintf(stderr,"received line %s\n",request_line);
        free(request_line);
    }
}

static void
event_cb(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR)
        perror("Error from bufferevent");
    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
}

int req_on_message_begin (http_parser* parser) {
    printf("enter %s\n",__func__);
    return 0;
}
int req_on_url (http_parser* parser, const char *at, size_t length) {
    printf("enter %s\n",__func__);
    return 0;
}
int req_on_header_field (http_parser* parser, const char *at, size_t length) {
    printf("enter %s\n",__func__);
    return 0;
}
int req_on_header_value (http_parser* parser, const char *at, size_t length) {
    printf("enter %s\n",__func__);
    return 0;
}
int req_on_headers_complete (http_parser* parser)      {
    printf("enter %s\n",__func__);
    return 0;
}
int req_on_body (http_parser* parser, const char *at, size_t length) {
    printf("enter %s\n",__func__);
    return 0;
}
int req_on_message_complete (http_parser* parser)      {
    printf("enter %s\n",__func__);
    return 0;
}

void destory_client_handler(client_handler* p){
    //do nothing now
}

client_handler* create_client_header(){
    client_handler* ret;
    ret=(client_handler*)malloc(sizeof(client_handler));
    http_parser_init(&ret->req_parser, HTTP_REQUEST);
    http_parser_init(&ret->res_parser, HTTP_RESPONSE);
    memset(&ret->parse_req_settings,0,sizeof(http_parser_settings));
    memset(&ret->parse_res_settings,0,sizeof(http_parser_settings));
    ret->parse_req_settings.on_message_begin=&req_on_message_begin;
    ret->parse_req_settings.on_url=&req_on_url;
    ret->parse_req_settings.on_header_field=&req_on_header_field;
    ret->parse_req_settings.on_header_value=&req_on_header_value;
    ret->parse_req_settings.on_headers_complete=&req_on_headers_complete;
    ret->parse_req_settings.on_body=&req_on_body;
    ret->parse_req_settings.on_message_complete=&req_on_message_complete;
    
    
    return ret;
}

static void my_acceptcb(struct evconnlistener *listener,
                 evutil_socket_t sock, struct sockaddr *addr, int addrlen, void *ptr){
    struct event_base *base;
    char host[NI_MAXHOST];
    int rv;
    
    rv = getnameinfo(addr, addrlen, host, sizeof(host), 0, 0, NI_NUMERICHOST);
    if(rv){
        fprintf(stderr,"getnameinfo() failed: %s\n",gai_strerror(rv));
        return ;
    }
    fprintf(stderr,"got connection from %s\n",host);
    base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
    client_handler* clienthandler=create_client_header();
    bufferevent_setcb(bev, read_cb, NULL, event_cb, clienthandler);
    
    bufferevent_enable(bev, EV_READ|EV_WRITE);
}

static void
accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d (%s) on the listener. "
            "Shutting down.\n", err, evutil_socket_error_to_string(err));
    
    event_base_loopexit(base, NULL);
}

/**
 * 创建evconnlistener
 */
static struct evconnlistener* create_evlistener(struct event_base *base,int family,configuration* config)
{
    // TODO Listen both IPv4 and IPv6 ?
    struct addrinfo hints;
    int fd = -1;
    int r;
    char service[10];
    struct addrinfo *res, *rp;

    int val = 1;
    
    snprintf(service, sizeof(service), "%u", config->listen_port);
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = family;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    
    r = getaddrinfo(config->listen_ip, service, &hints, &res);
    if(r != 0) {
        /* LOG(INFO) << "Unable to get IPv" << (family == AF_INET ? "4" : "6")
        << " address for " << get_config()->host << ": "
        << gai_strerror(r);*/
        return NULL;
    }
    
    //把getaddrinfo返回的addrinfo挨个尝试一遍
    for(rp = res; rp; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(fd == -1) {
            continue;
        }       
        if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val,
                      (socklen_t)(sizeof(val))) == -1) {
            close(fd);
            continue;
        }
        evutil_make_socket_nonblocking(fd);
        
        if(bind(fd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }
        close(fd);
    }
    if(rp) {
        char host[NI_MAXHOST];
        r = getnameinfo(rp->ai_addr, rp->ai_addrlen, host, sizeof(host),
                        0, 0, NI_NUMERICHOST);
        if(r == 0) {
            fprintf(stdout,"Listening on %s port %d \n",config->listen_ip,config->listen_port);
        } else {
            fprintf(stdout,"err,%s",gai_strerror(r));
            exit(-1);
        }
    }
    freeaddrinfo(res);
    if(rp == 0) {
        return NULL;
    }
    
    struct evconnlistener *evlistener = evconnlistener_new
    (base,
     my_acceptcb,//listener callback
     NULL, //TODO: pass handler
     LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
     10, //TODO: setting it in config file. this is the backlog param in listen(2) system call
     fd);
    evconnlistener_set_error_cb(evlistener, accept_error_cb);
    return evlistener;
}

int main(int argc, char **argv)
{
	struct event_base *base;
    struct evconnlistener *listener;
    //struct event *signal_event;

    //struct sockaddr_in sin;
    pid_t  pid, sid;
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

    if (config.run_daemon) {
        pid = fork();
        if (pid < 0) {
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            exit(EXIT_SUCCESS);
        }
        if ((sid = setsid()) < 0) {
            exit(EXIT_FAILURE);
        }
        if((chdir("/")) < 0){
            exit(EXIT_FAILURE);
        }
        umask(0);
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    }

    base = event_base_new();
    listener=create_evlistener(base,AF_INET,&config);
    if(listener){
        fprintf(stdout,"create listener successed, enter loop\n");
    }
    event_base_loop(base, 0);
    if(listener) {
        evconnlistener_free(listener);
    }
    return 0;
}
