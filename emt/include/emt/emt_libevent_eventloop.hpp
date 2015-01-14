#ifndef _HEADER_FILE_LIBEVENT_EVENTLOOP_HPP_
#define _HEADER_FILE_LIBEVENT_EVENTLOOP_HPP_

#include "emt/emt_include_libevent.hpp"
#include "emt/emt_util_network.hpp"
#include "emt/emt_include_cpp.hpp"
#include "emt/emt_io_cb.hpp"
#include "emt/emt_queue.hpp"
#include "emt/emt_notify.hpp"
#include "emt/emt_libevent_listener.hpp"
#include "emt/emt_session.hpp"
#include "emt/emt_libevent_wrapper.hpp"


namespace emt
{
    
static emt_funcIocb _readcb;
static emt_funcIocb _notifycb;
static emt_funcIocb _writecb;
static emt_funcErrorcb _errorcb;
static emt_funcTimecb _timecb;
static emt_funcAcceptcb _acceptcb;
    
class EventLoop
{
public:
    typedef std::tr1::unordered_map < int, T_Session > T_Sessions;
    typedef std::map < int, T_Queue * >  T_Queues;
    
    EventLoop(const std::map < std::string, IpPort > & bindinfo,
             unsigned workerCount):
                _inited(false),
                _bindinfo(bindinfo),
                _workerCount(workerCount),
                _base(NULL)
    {
    }
    ~EventLoop()
    {
        if(_base)
            event_base_free(_base);
        _listeners.clear();
        _sessions.clear();
        _tid_notify.clear();
        _fd_notify.clear();
        _tid_queue.clear();
    }
        
    int Init(emt_funcIocb my_readcb,
            emt_funcIocb my_notifycb,
            emt_funcIocb my_writecb,
            emt_funcErrorcb my_errorcb,
            emt_funcTimecb my_timecb,
            emt_funcAcceptcb my_acceptcb,
            std::string & errorstr)
    {
        if(_inited)
            return 1;
            
        _readcb =my_readcb;
        _notifycb =my_notifycb;
        _writecb =my_writecb;
        _errorcb =my_errorcb;
        _timecb =my_timecb;
        _acceptcb =my_acceptcb;
    
        _base =event_base_new();
        int rt =CreateEventListener(_bindinfo,
                                    _base,
                                    _acceptcb,
                                    this,
                                    _listeners,
                                    errorstr);
        if(0!=rt)
        {
            return -1;
        }
        _inited =true;
        return 0;
    }

    static std::map < std::string, IpPort > GenBindInfo(int port)
    {
        IpPort ipport;
        ipport.port =port;
        std::map < std::string, IpPort > bindinfo;
        bindinfo["front-end"] =ipport;
        return bindinfo;
    }

    T_Queue * GenQueue(int thread_id,std::string errorstr)
    {
        T_Queues ::iterator it =_tid_queue.find(thread_id);
        if(it!=_tid_queue.end())
        {
            return it->second;
        }
        T_Queue * queue =_queueFactory.GetInst();
        _tid_queue[thread_id] =queue;
        return queue;
    }

    Notify * GenNotify(int thread_id,std::string errorstr)
    {
        std::map < int, Notify * > ::iterator it =_tid_notify.find(thread_id);
        if(it!=_tid_notify.end())
        {
            return it->second;
        }

        Notify * notify =_notifyfactory.GetInst();
        T_Session output_session;
        int rt =CreateSessionByFd(_base,notify->GetReadFd(),
                      _notifycb,NULL,_errorcb,
                      this,output_session, errorstr);
        if(0!=rt)
        {
            return NULL;
        }
        _sessions[notify->GetReadFd()] =output_session;        
        _tid_notify[thread_id] =notify;
        _fd_notify[notify->GetReadFd()] =notify;
        return notify;
    }

    Notify * GetNotifyByFd(int fd)
    {
        std::map < int, Notify * > ::iterator it =_fd_notify.find(fd);
        if(it!=_fd_notify.end())
        {
            return it->second;
        }
        return NULL;
    }

    int Start(std::string & errorstr)
    {
        if (false==_inited)
        {
            printf("Start, init failed \n");
            return -1;
        }
            
        event_base_dispatch(_base);
        return 0;
    }

    void DumpSession()
    {
        printf("--------\nDumps:  sessions.size=%d,session is: \n    ",int(_sessions.size()));
        for (T_Sessions ::iterator it =_sessions.begin();
            it!=_sessions.end();it++)
        {
            printf("%d, ",it->first);
        }
        printf("\n--------\n");
    }
    
    
    int InsertSession(int fd,T_Session session)
    {
        T_Sessions ::iterator it =_sessions.find(fd);
        if(it!=_sessions.end())
        {
            _sessions.erase(it);
        }
        _sessions.insert(std::make_pair(fd,session));
        
//        printf("InsertSession , fd=%d \n",fd);
//        DumpSession();
        return 0;
    }
    int GetSession(int fd,T_Session & session)
    {
//        printf("GetSession , fd=%d \n",fd);
//        DumpSession();
        
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
        T_Sessions ::iterator it =_sessions.find(fd);
        if(it!=_sessions.end())
        {
            _sessions.erase(it);
            return 0;
        }
        return -1;
    }

    event_base * GetBase()
    {
        return _base;
    }

    std::map < int, T_Queue * > & GetQueues()
    {
        return _tid_queue;
    }

    void DumpNotify()
    {
        printf("--------\nDumps:  _fd_notify.size=%d,_fd_notify is: \n    ",int(_fd_notify.size()));
        for (std::map < int, Notify * > ::iterator it =_fd_notify.begin();
            it!=_fd_notify.end();it++)
        {
            printf("%d, ",it->first);
        }
        printf("\n--------\n");
    }
    Notify * GetNotify(int fd)
    {
//        printf("GetNotify, fd=%d \n",fd);
//        DumpNotify();
        std::map < int, Notify * > ::iterator it =_fd_notify.find(fd);
        if(it!=_fd_notify.end())
        {
            return it->second;
        }
        return NULL;
    }
    
private:
    bool _inited;
    std::map < std::string, IpPort > _bindinfo;
    unsigned _workerCount;
    event_base * _base;
    
    NotifyFactory _notifyfactory;
    QueueFactory _queueFactory;

    std::map < std::string, T_ListenerCtx > _listeners; // value is shared_ptr,auto delete. 初始化Init时建立，生命期同EventLoop对象。
    T_Sessions _sessions; // sessionid->Session. value is shared_ptr,auto delete. 连接建立时session插入map，连接断开时从map删除。
    std::map < int, Notify * > _tid_notify; // thread id->Notify, worker to network. value 是原始指针，生命期同NotifyFactory，由其来delete.
    // fd->Notify, worker to network. 在notifycb时，用于获取有事件的notify.
    // 因为notofycb返回时只能从bev中获取到fd，所以由_tid_notify拿不到notify.
    // 另外，notofycb中的arg指针只有eventloop指针，所以无法获知哪个notify有事件了.
    std::map < int, Notify * > _fd_notify;
    T_Queues _tid_queue; // thread id->Queue, network to worker. value 是原始指针，生命期同QueueFactory，由其来delete.
};

} // namespace emt


#endif // _HEADER_FILE_LIBEVENT_EVENTLOOP_HPP_

