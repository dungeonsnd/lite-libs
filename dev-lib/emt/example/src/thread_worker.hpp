#ifndef _HEADER_FILE_THREAD_WORKER_HPP_
#define _HEADER_FILE_THREAD_WORKER_HPP_

#include "cppfoundation/cf_thread.hpp"
#include <arpa/inet.h>

class MyRunnable: public cf::Runnable
{
public:
    MyRunnable(emt::T_Queue * queue,emt::Notify * notify):
        _queue(queue),_notify(notify)
    {
    }
    ~MyRunnable() {}
    virtual cf_pvoid Run(cf_void *)
    {
        while(1)
        {
            emt::Package pkg =_queue->Get();
            
            emt::Package rtpkg;
            rtpkg._fd =pkg._fd;
            rtpkg._unique =pkg._unique;
            rtpkg._data.assign(pkg._data.size()+4,'\0');
            uint32_t l =ntohl(pkg._data.size());
            memcpy(&rtpkg._data[0],&l,4);
            memcpy(&rtpkg._data[4],pkg._data.c_str(),pkg._data.size());
            _notify->Send(rtpkg);
        }
        return NULL;
    }
    
private:
    emt::T_Queue * _queue;
    emt::Notify * _notify;
};

#endif // _HEADER_FILE_THREAD_WORKER_HPP_
