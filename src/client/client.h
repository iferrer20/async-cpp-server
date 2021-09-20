#pragma once
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cstdint>
#include <list>
#include <memory>
#include "http.h"

#ifdef USE_SSL
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#else
#define WOLFSSL_CTX void
#endif

enum ClientState {
    B_RECV_DATA,
    B_RESP_DATA,
    RECV_DATA,
    RESP_DATA,
    CLOSED
};

struct Client;
using cd_it = std::list<std::unique_ptr<Client>>::const_iterator;

struct Client {
    const cd_it it;
    const int fd;
    const struct sockaddr_in addr;
    ClientState status;
    HTTPData http_data;

#ifdef USE_SSL
    WOLFSSL* ssl;
#endif

    void read();
    void write();

    explicit Client(const cd_it it, const int fd, const struct sockaddr_in addr, WOLFSSL_CTX* ctx); 
    ~Client();

private:
    uint64_t read_buff_size;
    uint64_t write_buff_size;

};

