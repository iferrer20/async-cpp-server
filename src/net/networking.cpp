#include <arpa/inet.h>
#include <fcntl.h>

#include "networking.h"

bool accept_conn(const int server_fd, int* const client_fd, struct sockaddr_in* const client_addr) {
    struct sockaddr_in address;
    const int addrlen = sizeof(struct sockaddr_in);
    *client_fd = accept4(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen, SOCK_NONBLOCK);
    return *client_fd != -1;
}

void get_client_ipv4(struct sockaddr_in* address, char* buff) {
    inet_ntop(AF_INET, &address->sin_addr, buff, INET_ADDRSTRLEN);
}
void set_nonblock_socket(int socket) {
    fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK);
}
