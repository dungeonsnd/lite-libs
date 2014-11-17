#ifndef _HEADER_FILE_CACHE_CONNECT_CB_HPP_
#define _HEADER_FILE_CACHE_CONNECT_CB_HPP_

void connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        printf("Error: %s\n", c->errstr);
        return;
    }
    printf("Disconnected...\n");
}

#endif // _HEADER_FILE_CACHE_CONNECT_CB_HPP_
