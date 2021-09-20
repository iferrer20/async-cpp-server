#include "http.h"


HTTPData::HTTPData() {

}

HTTPData::~HTTPData() {

}
/*void HTTPData::read() {
#ifdef USE_SSL
    if (!ssl_connected) {
        return 1;
    }
    int rsize = wolfSSL_read(ssl, data, 1023);
#else
    int rsize = ::read(fd, data, 1023);
#endif

    size += rsize;
    if (size > 4 && (*(unsigned int*)&data[size - 4]) == 0x0A0D0A0D) {
        std::cout << buff;
	completed = true;
    }

    return rsize;
}
*/
