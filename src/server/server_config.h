#pragma once

struct server_config_t {
#ifdef USE_SSL
    const char* ssl_cert_path;
    const char* ssl_key_path;
#endif
    const char* listen_addr;
    int listen_port;
};
