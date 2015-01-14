#ifndef _HEADER_FILE_EST_UTIL_NETWORK_HPP_
#define _HEADER_FILE_EST_UTIL_NETWORK_HPP_

#include "est/est_include_system.hpp"
#include "est/est_include_cpp.hpp"

namespace est
{

struct AddrPort
{
    AddrPort():ip(0),port(0){}
    unsigned ip;
    unsigned port;
};

struct IpPort
{
    IpPort():ip(""),port(0){}
    std::string ip;
    unsigned port;
};

inline int Accept(int listener,unsigned & remoteip,int & remoteport,unsigned & localip,int & localport)
{
    sockaddr_in in_addr;
    socklen_t slen = sizeof(in_addr);
    int fd = accept(listener, (struct sockaddr*)&in_addr, &slen);
    if(fd>=0)
    {
        remoteip =(unsigned)(in_addr.sin_addr.s_addr);
        remoteport =int(ntohs(in_addr.sin_port));
        
        sockaddr_in in_addrLocal;
        socklen_t locallen = sizeof(in_addrLocal);
        int rt =getsockname(fd, (sockaddr*)(&in_addrLocal), &locallen);
        if(0!=rt)
        {
        }
        localip =(unsigned)(in_addrLocal.sin_addr.s_addr);
        localport =int(ntohs(in_addrLocal.sin_port));
    }
    return fd;
}

} // namespace est


#endif // _HEADER_FILE_EST_UTIL_NETWORK_HPP_

