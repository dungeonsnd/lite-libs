#ifndef _HEADER_FILE_EMT_QUEUE_HPP_
#define _HEADER_FILE_EMT_QUEUE_HPP_

#include "emt/emt_include_cpp.hpp"
#include "emt/emt_package.hpp"
#include "cppfoundation/cf_concurrency_queue.hpp"

namespace emt
{

typedef cf::TYPEQueue < Package > ::Queue  T_Queue;

class QueueFactory
{
public:
    QueueFactory()
    {
    }
    ~QueueFactory()
    {
        for (std::vector < T_Queue * >::iterator it =_queue.begin();
            it!=_queue.end();it++)
        {
            delete *it;
        }
    }
    
    // Attention! shared_ptr is not thread-safe, so return raw ptr. 
    //   The returned T_Queue will be shared by network-thread and worker-threads.
    T_Queue * GetInst()
    {
        T_Queue * queue =new T_Queue();
        _queue.push_back(queue);
        return queue;
    }
private:
    std::vector < T_Queue * > _queue;
};

} // namespace emt

#endif // _HEADER_FILE_EMT_QUEUE_HPP_
