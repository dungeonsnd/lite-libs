#ifndef _HEADER_FILE_EMT_NOTIFY_HPP_
#define _HEADER_FILE_EMT_NOTIFY_HPP_

#include "emt/emt_pipe.hpp"
#include "emt/emt_package.hpp"
#include "cppfoundation/cf_concurrency_queue.hpp"

namespace emt
{

// 每个工作线程都持有一个通知器(Notify)，用于从网络线程接收任务 及发送处理结果给网络线程。
// 工作线程与网络线程对通知器的实现一无所知，他们只是知道收到数据和发送数据。
// 收发数据的类型是 Package


class Notify
{
public:

    typedef cf::TYPEQueue < Package > ::Queue  T_Queue;

    Notify():
        _notifyPipe(new Pipe()),
        _queue(new T_Queue())
    {
    }
    ~Notify()
    {
        delete _notifyPipe;
        delete _queue;
    }
    
    void Send(const Package & package)
    {
        _queue->Put(package);        
        write(_notifyPipe->fd_w,"a",1);
    }
    
    Package Recv()
    {
        return _queue->TryGet();
    }
    
    int GetReadFd()
    {
        return _notifyPipe->fd_r;
    }
    
private:
    Pipe * _notifyPipe;
    T_Queue * _queue;
};


class NotifyFactory
{
public:
    NotifyFactory()
    {
    }
    ~NotifyFactory()
    {
        for (std::vector < Notify * >::iterator it =_notifies.begin();
            it!=_notifies.end();it++)
        {
            delete *it;
        }
    }
    
    // Attention! shared_ptr is not thread-safe, so return raw ptr. 
    //   The returned notify will be shared by network-thread and worker-threads.
    Notify * GetInst()
    {
        Notify * notify =new Notify();
        _notifies.push_back(notify);
        return notify;
    }
private:
    std::vector < Notify * > _notifies;
};

} // namespace emt

#endif // _HEADER_FILE_EMT_NOTIFY_HPP_

