#include <string.h>
#include "epoll_loop.h"
#include "../net/networking.h"
#include "../server/server.h"

Loop::Loop() : 
    _stop{false},
    num_ready_polls{0},
    current_ready_poll{0},
    epoll{epoll_create1(0)} {

    
}

Loop::~Loop() {

}
void Loop::stop() {
    _stop = true;
}
static Client* get_client(struct epoll_event* poll) {
    return static_cast<Client*> (poll->data.ptr);
}

void Loop::del_client_poll(struct epoll_event* poll) {
    Client* client = get_client(poll);
    epoll_ctl(epoll, EPOLL_CTL_DEL, client->fd, 0);
}

void Loop::add_client_poll(struct epoll_event* poll) {
    Client* client = get_client(poll);
    epoll_ctl(epoll, EPOLL_CTL_ADD, client->fd, poll); 
}
void Loop::add_server_poll() {
    struct epoll_event poll; 
    memset(&poll, 0, sizeof(struct epoll_event));
    poll.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    //poll.data.fd = server->fd;
    epoll_ctl(epoll, EPOLL_CTL_ADD, server->fd, &poll); 
}
void Loop::del_server_poll() {
    epoll_ctl(epoll, EPOLL_CTL_DEL, server->fd, 0);
}

void Loop::run() { 
    add_server_poll();
    while (!_stop) {
        num_ready_polls = epoll_wait(epoll, ready_polls, 1024, 2000);
        
        for (current_ready_poll = 0; current_ready_poll < num_ready_polls; current_ready_poll++) {
            struct epoll_event* poll = &ready_polls[current_ready_poll];
            int events = poll->events;

            if (!poll->data.ptr) {
                if (events & EPOLLIN) {
                    struct epoll_event poll;
                    poll.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
                    int client_fd;
                    struct sockaddr_in client_addr;
                    if (accept_conn(server->fd, &client_fd, &client_addr)) {
                        poll.data.ptr = server->new_client(client_fd, &client_addr);
                        add_client_poll(&poll);
                    }
                }
            } else {
                Client* client = get_client(poll);
                if (events & EPOLLIN) { 
                    if (!client->read()) {
                        del_client_poll(poll);
                        server->del_client(client);
                        continue;
                    }
                }
                if (events & EPOLLOUT) {
                    client->write();
                }
                if (events & EPOLLIN && events & EPOLLOUT) {
                    client->readwrite();
                }
                if (events & EPOLLRDHUP) {
                    del_client_poll(poll);
                    server->del_client(client);
                    continue;
                }
            }
        }
        server->log();
    }

    del_server_poll();
}

