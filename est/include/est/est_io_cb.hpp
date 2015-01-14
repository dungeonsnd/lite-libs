#ifndef _HEADER_FILE_EST_IO_CB_HPP_
#define _HEADER_FILE_EST_IO_CB_HPP_

typedef void (*est_funcIocb) (bufferevent *bev, void *arg);
typedef void (*est_funcErrorcb) (bufferevent *bev,short event, void *arg);
typedef void (*est_funcTimecb) (int fd,short event, void *arg);
typedef void (*est_funcAcceptcb) (evutil_socket_t listener, short event, void *arg);

#endif // _HEADER_FILE_EST_IO_CB_HPP_
