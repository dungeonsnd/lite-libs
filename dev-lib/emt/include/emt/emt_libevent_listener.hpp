#ifndef _HEADER_FILE_LIBEVENT_LISTENER_HPP_
#define _HEADER_FILE_LIBEVENT_LISTENER_HPP_

#include "emt/emt_include_libevent.hpp"
#include "emt/emt_io_cb.hpp"

namespace emt
{

class ListenerCtx
{
public:
    event * _listener;
    
    ListenerCtx(event_base * base, 
                evutil_socket_t listenerfd, 
                emt_funcAcceptcb acceptcb,
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

} // namespace emt

#endif // _HEADER_FILE_LIBEVENT_LISTENER_HPP_

