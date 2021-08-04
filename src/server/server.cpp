#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory>
#include <signal.h>
#include <string.h>

#include "server.h"
#include "../net/networking.h"

Server::Server(server_config_t sv_config) : 
    sv_config{sv_config}, 
    fd{socket(AF_INET, SOCK_STREAM, 0)} {
    
}

int Server::run() {
    int opt = 1; 
    
    // Creating socket file descriptor 
    if (fd == 0) { 
        std::cerr << "socket failed\n";
        exit(1);
    }
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt\n";
        exit(1);
    }
    set_nonblock_socket(fd);

    struct sockaddr_in listen_addr_sock;
    listen_addr_sock.sin_family = AF_INET;
    listen_addr_sock.sin_addr.s_addr = inet_addr(sv_config.listen_addr);
    listen_addr_sock.sin_port = htons(sv_config.listen_port);

    // Forcefully attaching socket to the port 8080 
    if (bind(fd, (struct sockaddr *)&listen_addr_sock, sizeof(listen_addr_sock))<0) {
        std::cerr << "Bind failed\n";
        exit(1);
    } 
    if (listen(fd, SOMAXCONN) < 0) { 
        std::cerr << "Listen\n";
        exit(1);
    }

#ifdef USE_SSL
    init_ssl();
#endif

    loop.run();
    return 0;
}

Client* Server::new_client(const int client_fd, const struct sockaddr_in* client_addr) {
    auto it = clients.emplace(clients.end());
    (*it) = std::make_unique<Client>(
            it, 
            client_fd, 
            *client_addr,
#ifdef USE_SSL
            ssl_ctx
#else
            nullptr
#endif      
            );
    return it->get();
}
void Server::del_client(Client* client) {
    clients.erase(client->it);
}

void Server::stop() {
    std::cout << "Stoping.." << std::endl;
    loop.stop();
}

void Server::log() {
    std::cout << "Clients: " << clients.size() << std::endl;
}


Server::~Server() {
#ifdef USE_SSL
    wolfSSL_CTX_free(ssl_ctx);
#endif
}
