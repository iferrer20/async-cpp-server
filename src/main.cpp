#include <iostream>
#include <signal.h>
#include <functional>
#include "server/server.h"

std::unique_ptr<Server> server;

int main() {
    server = std::make_unique<Server>(server_config_t {
#ifdef USE_SSL
            .ssl_cert_path = "certs/cert.pem",
            .ssl_key_path = "certs/key.pem",
#endif
            .listen_addr = "0.0.0.0",
            .listen_port = 8080
    });
    auto stop_server = +[] (int signum) {
        server->stop();
    };
    signal(SIGTERM, stop_server);
    signal(SIGINT, stop_server);
    server->run();

    return 0;
}
