/* 
 * goagent-client is a new implementation of GoAgent client with pure C language
 * Under GPLv2 License
 */

#ifndef __GOAGENT_CLIENT_H
#define __GOAGENT_CLIENT_H

typedef struct {
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

    const char* profile_mode;
    int profile_window;
    const char* profile_hosts;
    const char* profile_sites;
    const char* profile_forcehttps;
    const char* profile_withgae;
    const char* profile_withdns;

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
} proxy_configuration;

extern int ini_file_handler (void* pconfig, const char* section, 
                             const char* key, const char* value);

#endif /* __GOAGENT_CLIENT_H */
