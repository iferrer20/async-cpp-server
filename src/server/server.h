#pragma once
#include <netinet/in.h> 
#include <memory>
#ifdef USE_SSL
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#endif

#include "server_config.h"
#include "../client/client.h"
#include "../polling/epoll_loop.h"

struct Server {

    const server_config_t sv_config;
    const int fd;

    Loop loop;
    std::list<std::unique_ptr<Client>> clients;

    int run();
    void stop();
    void log();

    Client* new_client(const int client_fd, const struct sockaddr_in* client_addr);
    void del_client(Client* client);

    explicit Server(server_config_t sv_config);
    ~Server();

#ifdef USE_SSL
    WOLFSSL_CTX* ssl_ctx;
    void init_ssl();
#endif
};

extern std::unique_ptr<Server> server;
