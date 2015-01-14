#ifndef _HEADER_FILE_EMT_LIBEVENT_WRAPPER_HPP_
#define _HEADER_FILE_EMT_LIBEVENT_WRAPPER_HPP_

#include "emt/emt_include_libevent.hpp"
#include "emt/emt_libevent_listener.hpp"
#include "emt/emt_io_cb.hpp"
#include "emt/emt_session.hpp"

namespace emt
{

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

inline int CreateEventListener(const std::map < std::string, IpPort > & bindinfo,
                        event_base * base,
                        emt_funcAcceptcb acceptcb,
                        void * ctx,
                        std::map < std::string, T_ListenerCtx > & listeners, 
                        std::string & errorstr)
{
    int ret =0;
    do{
        int rt =0;
        for (std::map < std::string, IpPort >::const_iterator it =bindinfo.begin();
            it!=bindinfo.end();it++)
        {
            evutil_socket_t listener =0;
            rt =CreateListenerFd(it->second.ip,it->second.port,listener,errorstr);
            if (0!=rt)
            {
                ret =-1;
                break;
            }
            
            T_ListenerCtx listenerCtx(new ListenerCtx(base, listener, acceptcb, ctx)); // shared_ptr, auto delete.
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


inline int CreateSessionByFd(event_base * base,int fd,
                      emt_funcIocb readcb,emt_funcIocb writecb,emt_funcErrorcb errorcb,
                      void * arg,
                      T_Session & output_session,
                      std::string & errorstr)
{
    int ret =0;
    do{
        evutil_make_socket_nonblocking(fd);
        bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
        if(NULL==bev)
        {
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
        T_Session session(new SessionBase(localPort,remotePort,localIp,remoteIp,connectedTime,bev));
        output_session =session;
        
        bufferevent_setcb(bev, readcb, writecb, errorcb, arg);
    }while(0);
    return ret;
}

} // namespace emt


#endif // _HEADER_FILE_EMT_LIBEVENT_WRAPPER_HPP_

