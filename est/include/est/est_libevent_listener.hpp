#ifndef _HEADER_FILE_EST_LIBEVENT_LISTENER_HPP_
#define _HEADER_FILE_EST_LIBEVENT_LISTENER_HPP_

#include "est/est_include_libevent.hpp"
#include "est/est_io_cb.hpp"

namespace est
{

class ListenerCtx
{
public:
    event * _listener;
    
    ListenerCtx(event_base * base, 
                evutil_socket_t listenerfd, 
                est_funcAcceptcb acceptcb,
                void * ctx):
                _listener(NULL)
    {
        _listener = event_new(base, listenerfd, EV_READ|EV_PERSIST, acceptcb, ctx);
        event_add(_listener, NULL);
    }
    ~ListenerCtx()
    {
        if(_listener)
            event_free( _listener );
    }
};

typedef std::tr1::shared_ptr < ListenerCtx > T_ListenerCtx;

} // namespace est

#endif // _HEADER_FILE_EST_LIBEVENT_LISTENER_HPP_

