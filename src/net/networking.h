#pragma once
#include <netinet/in.h>
#include <memory>

#include "../client/client.h"

bool accept_conn(const int server_fd, int* const client_fd, struct sockaddr_in* const client_addr);
void set_nonblock_socket(int socket);
void get_client_ipv4(struct sockaddr_in* address, char* buff);
