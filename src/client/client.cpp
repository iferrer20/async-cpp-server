#include <iostream>
#include <unistd.h>
#include <vector>
#include <string_view>
#include "client.h"

Client::Client(const cd_it it, const int fd, const struct sockaddr_in addr, WOLFSSL_CTX* ctx) : 
    it{it},
    fd{fd},
    addr{addr},
    status{B_RECV_DATA},
#ifdef USE_SSL
    ssl{wolfSSL_new(ctx)},
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

void Client::read() {

    if (status == B_RECV_DATA) {
#ifdef USE_SSL
        if (wolfSSL_accept(ssl) == SSL_SUCCESS) {
            // Ready for reciving data
            status = RECV_DATA;
        }
        return;
#else
        status = RECV_DATA;
#endif
    }

    // Read data
    int size;
#ifdef USE_SSL
    size = wolfSSL_read(fd, http_data.data + read_buff_size, MAX_BUFFERING - read_buff_size);
#else
    size = ::read(fd, http_data.data + read_buff_size, MAX_BUFFERING - read_buff_size);
#endif

    std::cout << "READ " << size << "\n";
    std::cout << http_data.data + read_buff_size << "\n";

    // Check end of headers
    std::string_view s (http_data.data + read_buff_size, size); 

    read_buff_size += size;

    if (size <= 0) {
        status = CLOSED;
    }


}

void Client::write() {
}

Client::~Client() {
    std::cout << "Bye client\n";
#ifdef USE_SSL
    wolfSSL_shutdown(ssl);
    wolfSSL_free(ssl);
#endif
    close(fd);
}
