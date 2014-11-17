#ifndef _HEADER_FILE_CACHE_ASYNC_HPP_
#define _HEADER_FILE_CACHE_ASYNC_HPP_

#include "include_libevent_async.hpp"

static redisConnectCallback * _connectCb;
static redisDisconnectCallback * _disconnectCb;

class CacheAsync
{
public:
    CacheAsync(event_base * base):
        _base(base),
        _ctx(NULL)
    {
        _connectCb =NULL;
        _disconnectCb =NULL;
    }
    ~CacheAsync()
    {
    }
    
    int AsyncConnect(const char * ip,int port) // TODO: Add strerror.
    {    
        _ctx = redisAsyncConnect(ip, port);
        if(NULL==_ctx)
        {
            return 1;
        }
        else if (_ctx->err) {
            /* Let _ctx leak for now... */
            printf("Error: %s\n", _ctx->errstr);
            return 2;
        }
        else
        {
            redisLibeventAttach(_ctx,_base);
            return 0;
        }
    }
    
    void SetConnectCallback(redisConnectCallback connectCb, redisDisconnectCallback disconnectCb)
    {
        _connectCb =connectCb;
        _disconnectCb =disconnectCb;
        
        redisAsyncSetConnectCallback(_ctx,_connectCb);
        redisAsyncSetDisconnectCallback(_ctx,_disconnectCb);
    }
    
    int AsyncCommand(redisCallbackFn *fn, void *privdata, const char *format, ...)
    {    
        va_list valp;        
        va_start(valp,format);
        int rt=redisAsyncCommand(_ctx, fn, privdata, format, valp);
        va_end(valp);    
        return rt;
    }
    
private:
    event_base * _base;
    redisAsyncContext * _ctx;
};

#endif // _HEADER_FILE_CACHE_ASYNC_HPP_