#ifndef _HEADER_FILE_EST_LIBEVENT_EVENTLOOP_HPP_
#define _HEADER_FILE_EST_LIBEVENT_EVENTLOOP_HPP_

#include "est/est_include_libevent.hpp"
#include "est/est_util_network.hpp"
#include "est/est_include_cpp.hpp"
#include "est/est_io_cb.hpp"
#include "est/est_libevent_listener.hpp"
#include "est/est_session.hpp"
#include "est/est_libevent_wrapper.hpp"


namespace est
{


class EventLoop
{
public:
    typedef std::tr1::unordered_map < int, T_Session > T_Sessions;
    typedef std::map < int, IpPort > T_BindInfo;
        
    EventLoop(const T_BindInfo & bindinfo):
                _inited(false),
                _base(NULL)
    {
        _base =event_base_new();
        
        for(T_BindInfo::const_iterator it =bindinfo.begin();
            it!=bindinfo.end();it++)
        {
            AcceptcbArg * arg =new AcceptcbArg();
            arg->ipPort =it->second;
            arg->specId =it->first;
            arg->base =_base;
            _listenInfo[it->first] =arg;
        }
    }
    ~EventLoop()
    {
        if(_base)
            event_base_free(_base);
        _listeners.clear();
        _sessions.clear();
        
        for(T_ListenInfo::const_iterator it =_listenInfo.begin();
            it!=_listenInfo.end();it++)
            delete it->second;
    }
        
    int Init(est_funcAcceptcb acceptcb,std::string & errorstr)
    {
        if(_inited)
            return 1;
        int rt =CreateEventListener(_listenInfo,
            _base,acceptcb,_listeners,errorstr);
        if(0!=rt)
            return -1;
        _inited =true;
        return 0;
    }

    int Start(est_funcAcceptcb acceptcb,std::string & errorstr)
    {
        if (-1==Init(acceptcb,errorstr))
            return -1;
        event_base_dispatch(_base);
        return 0;
    }
        
    int InsertSession(int fd,T_Session session)
    {
        _sessions.erase(fd);
        _sessions.insert(std::make_pair(fd,session));
        return 0;
    }
    
    int GetSession(int fd,T_Session & session)
    {
        T_Sessions ::iterator it =_sessions.find(fd);
        if(it!=_sessions.end())
        {
            session =it->second;
            return 0;
        }
        return -1;
    }
    
    int RemoveSession(int fd)
    {
        return 1==_sessions.erase(fd);
    }

private:
    bool _inited;
    T_ListenInfo _listenInfo;
    event_base * _base;

    std::map < int, T_ListenerCtx > _listeners; // key is specId,value is shared_ptr,auto delete. 初始化Init时建立，生命期同EventLoop对象。
    T_Sessions _sessions; // sessionid->Session. value is shared_ptr,auto delete. 连接建立时session插入map，连接断开时从map删除。
};

} // namespace est


#endif // _HEADER_FILE_EST_LIBEVENT_EVENTLOOP_HPP_

