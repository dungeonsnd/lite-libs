#ifndef _HEADER_FILE_EST_SESSION_BASE_HPP_
#define _HEADER_FILE_EST_SESSION_BASE_HPP_

#include "est/est_include_libevent.hpp"
#include "est/est_include_system.hpp"
#include "est/est_include_cpp.hpp"

namespace est
{

class SessionBase
{
public:
    enum Status
    {
        STATUS_RECVING_HEADER =1000,
        STATUS_RECVING_BODY
    };

    Status _status;
    int _specid; // 标识这个session的识别号，如front-end,back-end,mantainance.
    int _bodySize;
    int _localPort;
    int _remotePort;
    unsigned _localIp;
    unsigned _remoteIp;
    int _connectedTime;
    bufferevent * _bev;
    
    SessionBase(int specid,
            int localPort,
            int remotePort,
            int localIp,
            int remoteIp,
            int connectedTime,
            bufferevent * bev):
        _status(STATUS_RECVING_HEADER),
        _specid(specid),
        _bodySize(0),
        _localPort(localPort),
        _remotePort(remotePort),
        _localIp(localIp),
        _remoteIp(remoteIp),
        _connectedTime(connectedTime),
        _bev(bev)
    {
    }
    ~SessionBase()
    {
        if(_bev)
            bufferevent_free(_bev);
    }
};


typedef SessionBase Session;
typedef std::tr1::shared_ptr < SessionBase > T_Session;
    
} // namespace est


#endif // _HEADER_FILE_EST_SESSION_BASE_HPP_
