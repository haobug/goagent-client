/* 
 * goagent-client is a new implementation of GoAgent client with pure C language
 * Under GPLv2 License
 */

#ifndef __GOAGENT_CLIENT_H
#define __GOAGENT_CLIENT_H

//http_parser的文件参见https://github.com/joyent/http-parser
#include "http_parser.h"
//这是一个用宏写的C语言的链表库
#include "myqueue.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct profile_struct {
    const char *name;
    const char* mode;
    int window;
    const char* hosts;
    const char* sites;
    const char* forcehttps;
    const char* withgae;
    const char* withdns;
    struct profile_struct* next;
} profile;

typedef struct configuration_struct{
    int run_daemon;

    const char* listen_ip;
    int listen_port;
    int listen_visible;
    int listen_debuginfo;

    const char* gae_appid;
    const char* gae_password;
    const char* gae_path;
    const char* gae_profile;
    int gae_crlf;
    int gae_validate;

    int pac_enable;
    const char* pac_ip;
    int pac_port;
    const char* pac_file;
    const char* pac_gfwlist;

    int paas_enable;
    const char* paas_password;
    const char* paas_listen;
    const char* paas_fetchserver;

    int proxy_enable;
    int proxy_autodetect;
    const char* proxy_host;
    int proxy_port;
    const char* proxy_username;
    const char* proxy_password;

    const char* autorange_hosts;
    int autorange_threads;
    int autorange_maxsize;
    int autorange_waitsize;
    int autorange_bufsize;

    int crlf_enable;
    const char* crlf_dns;
    const char* crlf_sites;

    int dns_enable;
    const char* dns_listen;
    const char* dns_remote;
    int dns_cachesize;
    int dns_timeout;

    int socks5_enable;
    const char* socks5_password;
    const char* socks5_listen;
    const char* socks5_fetchserver;

    int useragent_enable;
    const char* useragent_string;

    profile* profiles;
    profile* current_profile;
} configuration;


typedef struct  {
    char* str;
    size_t len;
    size_t allocated_len;
}c_string;
    
    /**
     * malloc a new c_string,with allocated_len=len
     * \param len the initial allocated_len
     */
    extern c_string* c_string_sized_new(size_t len);
    /**
     * Adds a string onto the end of a c_string, expanding it if necessary.
     * \param string a c_string
     * \param val the string to append onto the end of string
     * \return same as the input string pointer
     */
    extern c_string* c_string_append(c_string* string,const char* val);

    extern void c_string_free(c_string* p);
    
    /**
     * HTTP头部的一个key、value对
     */
    typedef struct http_header_param_{
        c_string* key;
        c_string* value;
        //point to the next entry
        TAILQ_ENTRY(http_header_param_) q;
    }http_header_param;
    

    typedef struct{
        unsigned short http_major;
        unsigned short http_minor;
        unsigned char method;
        c_string* path;
        TAILQ_HEAD(headerlist, http_header_param) headers;
        c_string* body;
    }http_request_message;
    
    typedef struct{
        unsigned short http_major;
        unsigned short http_minor;
        unsigned short status_code;
        TAILQ_HEAD(headerlist_res, http_header_param) headers;
        c_string* body;
    }http_response_message;


/**
 * 从browser来的每个http connection对应一个client_hander
 */
typedef struct client_handler{
    //parse从browser发送给this daemon的http request
    http_parser req_parser;
    //parse从gae server发送给this daemon的http response
    http_parser res_parser;
    //req_parser的settings
    http_parser_settings parse_req_settings;
    //res_parser的settings
    http_parser_settings parse_res_settings;
    
    //parser的example见https://gist.github.com/ry/155877
    //我们此处还缺几个字段来记录parse的状态。比如收到header value的时候需要知道它对应的header key是谁
} client_handler;


extern client_handler* create_client_header();
extern void destory_client_handler(client_handler* p);
extern int ini_file_handler (void* pconfig, const char* section,
        const char* key, const char* value);
extern int getoption(int argc, char **argv, configuration* config);
extern profile* find_profile(const char* name, configuration* config,int create);
extern int switch_profile(configuration *config, const char* name);

#ifdef __cplusplus
}
#endif
#endif /* __GOAGENT_CLIENT_H */
