#ifndef _HEADER_FILE_EST_LIBEVENT_WRAPPER_HPP_
#define _HEADER_FILE_EST_LIBEVENT_WRAPPER_HPP_

#include "est/est_include_libevent.hpp"
#include "est/est_libevent_listener.hpp"
#include "est/est_io_cb.hpp"
#include "est/est_session.hpp"
#include "est/est_include_system.hpp"

namespace est
{


struct AcceptcbArg
{
    IpPort ipPort;
    int specId;
    event_base * base;
};
typedef std::map < int, AcceptcbArg* > T_ListenInfo; // specId->acceptcb arg.


inline int CreateListenerFd(const std::string & bindaddr,unsigned port,
                    evutil_socket_t & listener,
                      std::string & errorstr)
{
    int ret =0;
    do{
        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        if(bindaddr.size()<1)
            sin.sin_addr.s_addr =0;
        else
            sin.sin_addr.s_addr =inet_addr(bindaddr.c_str());
        sin.sin_port = htons(port);

        listener = socket(AF_INET, SOCK_STREAM, 0);
        evutil_make_socket_nonblocking(listener);

        ret =bind(listener, (struct sockaddr*)&sin, sizeof(sin));
        if (-1==ret)
        {
            if(errorstr.size()<1024)
                errorstr.resize(1024,'\0');
            snprintf(&errorstr[0],errorstr.size(),"Failed bind,errno=%d,%s",errno,strerror(errno));
            ret =1;
            break;
        }
        if (listen(listener, 1024)<0)
        {
            if(errorstr.size()<1024)
                errorstr.resize(1024,'\0');
            snprintf(&errorstr[0],errorstr.size(),"Failed listen,errno=%d,%s",errno,strerror(errno));
            ret =1;
            break;
        }
    }while(0);
    return ret;
}

inline int CreateEventListener(T_ListenInfo & _listenInfo,
                        event_base * base,
                        est_funcAcceptcb acceptcb,
                        std::map < int, T_ListenerCtx > & listeners, 
                        std::string & errorstr)
{
    int ret =0;
    do{
        int rt =0;
        for (T_ListenInfo::iterator it =_listenInfo.begin();
            it!=_listenInfo.end();it++)
        {
            evutil_socket_t listener =0;
            rt =CreateListenerFd(it->second->ipPort.ip,it->second->ipPort.port,listener,errorstr);
            if (0!=rt)
            {
                ret =-1;
                break;
            }
            
            AcceptcbArg * arg =it->second;
            T_ListenerCtx listenerCtx(new ListenerCtx(base, listener, acceptcb, arg)); // shared_ptr, auto delete.
            listeners[it->first] =listenerCtx;
        }
        if (-1==rt)
        {
            ret =1;
            break;
        }
    }while(0);
    return ret;
}


inline int CreateSessionByFd(event_base * base,int fd,int specId,int sessionId,
                      est_funcIocb readcb,est_funcIocb writecb,est_funcErrorcb errorcb,
                      T_Session & output_session,
                      std::string & errorstr)
{
    int ret =0;
    do{
        evutil_make_socket_nonblocking(fd);
        bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if(NULL==bev)
        {
            snprintf(&errorstr[0],errorstr.size(),"Failed bufferevent_socket_new");
            ret =1;
            break;
        }
        
        int options =EV_READ;    
        if (readcb) options |=EV_READ;
        if (writecb) options |=EV_WRITE;
        bufferevent_enable(bev, options);
        
        struct sockaddr_in local_addr;
        socklen_t local_len = sizeof(local_addr);
        int rt = getsockname(fd, (struct sockaddr*)&local_addr, &local_len);
        if(NULL==bev)
        {
            bufferevent_free(bev);
            if(errorstr.size()<1024)
                errorstr.resize(1024,'\0');
            snprintf(&errorstr[0],errorstr.size(),"Failed getsockname,errno=%d,%s",errno,strerror(errno));
            ret =1;
            break;
        }
        struct sockaddr_in peer_addr;
        socklen_t peer_len = sizeof(peer_addr);
        rt = getpeername(fd, (struct sockaddr*)&peer_addr, &peer_len);
        if(NULL==bev)
        {
            bufferevent_free(bev);
            if(errorstr.size()<1024)
                errorstr.resize(1024,'\0');
            snprintf(&errorstr[0],errorstr.size(),"Failed getpeername,errno=%d,%s",errno,strerror(errno));
            ret =1;
            break;
        }

        int localPort =int(local_addr.sin_port);
        int remotePort =int(peer_addr.sin_port);
        int localIp =int(local_addr.sin_addr.s_addr);
        int remoteIp =int(peer_addr.sin_addr.s_addr);
        int connectedTime =int(time(NULL));
        
        Session * psession =new SessionBase(specId,sessionId,localPort,remotePort,localIp,remoteIp,connectedTime,bev);
        output_session.reset(psession);     
        bufferevent_setcb(bev, readcb, writecb, errorcb, psession);
    }while(0);
    return ret;
}

} // namespace est


#endif // _HEADER_FILE_EST_LIBEVENT_WRAPPER_HPP_

