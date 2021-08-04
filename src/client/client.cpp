#include <iostream>
#include <unistd.h>
#include <vector>
#include "client.h"

Client::Client(const cd_it it, const int fd, const struct sockaddr_in addr, WOLFSSL_CTX* ctx) : 
    it{it},
    fd{fd},
    addr{addr},
#ifdef USE_SSL
    ssl{wolfSSL_new(ctx)},
    ssl_connected{false}, 
#endif
    read_buff_size{0}, 
    write_buff_size{0} {
    std::cout << "New client\n";
#ifdef USE_SSL 

    wolfSSL_set_fd(ssl, fd); 
    wolfSSL_SetIOWriteFlags(ssl, MSG_NOSIGNAL);
    //SSL_set_accept_state(ssl);
#endif

}

int Client::read() {

#ifdef USE_SSL
    if (!ssl_connected) {
        return 1;
    }
    char buff[1024];
    int size = wolfSSL_read(ssl, buff, 1023);
#else
    char buff[1024];
    int size = ::read(fd, buff, 1023); 
#endif

    if (size > 0) {
        buff[size] = 0;
        std::cout << buff;
    }
    return size;
}

int Client::write() {
    return 0;
}

void Client::readwrite() {
#ifdef USE_SSL
   if (!ssl_connected) {
        int ret = wolfSSL_accept(ssl);
        if (ret == SSL_SUCCESS) {
            ssl_connected = true;
        }
    }
#endif
}

Client::~Client() {
    std::cout << "Bye client\n";
#ifdef USE_SSL
    wolfSSL_shutdown(ssl);
    wolfSSL_free(ssl);
#endif
    close(fd);
}
