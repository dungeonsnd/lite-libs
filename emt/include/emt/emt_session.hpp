#ifndef _HEADER_FILE_EMT_SESSION_BASE_HPP_
#define _HEADER_FILE_EMT_SESSION_BASE_HPP_

#include "emt/emt_include_libevent.hpp"
#include "emt/emt_include_system.hpp"
#include "emt/emt_include_cpp.hpp"

namespace emt
{

class SessionBase
{
public:
    enum Status
    {
        STATUS_RECVING_HEADER =1000,
        STATUS_RECVING_BODY
    };

    int _unique;
    Status _status;
    int _bodySize;
    int _localPort;
    int _remotePort;
    unsigned _localIp;
    unsigned _remoteIp;
    int _connectedTime;
    bufferevent * _bev;
    
    SessionBase(int localPort,
            int remotePort,
            int localIp,
            int remoteIp,
            int connectedTime,
            bufferevent * bev):
        _unique(0),
        _status(STATUS_RECVING_HEADER),
        _bodySize(0),
        _localPort(localPort),
        _remotePort(remotePort),
        _localIp(localIp),
        _remoteIp(remoteIp),
        _connectedTime(connectedTime),
        _bev(bev)
    {
        srand((unsigned int)time(NULL));
        _unique =rand();
    }
    ~SessionBase()
    {
        if(_bev)
            bufferevent_free(_bev);
    }
};


typedef std::tr1::shared_ptr < SessionBase > T_Session;
    
} // namespace emt


#endif // _HEADER_FILE_EMT_SESSION_BASE_HPP_
