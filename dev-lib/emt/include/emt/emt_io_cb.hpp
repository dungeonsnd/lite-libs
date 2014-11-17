#ifndef _HEADER_FILE_EMT_IO_CB_HPP_
#define _HEADER_FILE_EMT_IO_CB_HPP_

typedef void (*emt_funcIocb) (bufferevent *bev, void *arg);
typedef void (*emt_funcErrorcb) (bufferevent *bev,short event, void *arg);
typedef void (*emt_funcTimecb) (int fd,short event, void *arg);
typedef void (*emt_funcAcceptcb) (evutil_socket_t listener, short event, void *arg);

#endif // _HEADER_FILE_EMT_IO_CB_HPP_
