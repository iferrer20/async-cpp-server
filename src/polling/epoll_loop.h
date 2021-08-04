#pragma once
#include <sys/epoll.h>
#include <list>
#include <memory>
#include "poll.h"

class Loop {
    bool _stop;
    int num_polls;
    int num_ready_polls;
    int current_ready_poll;
    struct epoll_event ready_polls[1024];
    
public:
    const int epoll;

    Loop();
    ~Loop();
    void del_client_poll(struct epoll_event* poll);
    void add_client_poll(struct epoll_event* poll);
    void add_server_poll();
    void del_server_poll();
    void run();
    void stop();
};

