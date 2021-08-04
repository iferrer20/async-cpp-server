#ifdef USE_SSL
#include "server.h"
#include <iostream>

void Server::init_ssl() {

    /* Init openssl libraries */ 
    wolfSSL_load_error_strings();
    wolfSSL_Debugging_ON();

    WOLFSSL_METHOD* meth = wolfTLSv1_2_server_method();
    ssl_ctx = wolfSSL_CTX_new(meth);

    if (!ssl_ctx) {
        std::cerr << "Error creating context.\n";
        exit(1);
    }

    /* ------------------------------- */
    /* Configure certificates and keys */
    if (wolfSSL_CTX_use_certificate_file(ssl_ctx, sv_config.ssl_cert_path, SSL_FILETYPE_PEM) != SSL_SUCCESS) {
        std::cerr << "Error using cert file\n";
        exit(1);
    }

    if (wolfSSL_CTX_use_PrivateKey_file(ssl_ctx, sv_config.ssl_key_path, SSL_FILETYPE_PEM) != SSL_SUCCESS) {
        std::cerr << "Error using key file\n";
        exit(1);
    }

    if (!wolfSSL_CTX_check_private_key(ssl_ctx)) {
        std::cerr << "Private key does not match public key in certificate.\n";
        exit(1);
    }

    wolfSSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, 0);
}
#endif
