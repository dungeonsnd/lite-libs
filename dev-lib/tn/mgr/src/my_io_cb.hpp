#ifndef _HEADER_FILE_MY_IO_CB_HPP_
#define _HEADER_FILE_MY_IO_CB_HPP_

#include "est/est.hpp"
#include "est/est_include_libevent.hpp"
#include "est/est_libevent_eventloop.hpp"

extern est::EventLoop * my_eventLoop;

inline void SetEventLoop(est::EventLoop * eventLoop)
{
    my_eventLoop =eventLoop;
}

void my_readcb(bufferevent *bev, void *arg);
void my_writecb(bufferevent *bev,void *ctx);
void my_errorcb(bufferevent *bev,short event, void *arg);
void my_timecb(int fd,short event,void *ctx);
void my_acceptcb(evutil_socket_t listener, short event, void *arg);


#endif // _HEADER_FILE_MY_IO_CB_HPP_
