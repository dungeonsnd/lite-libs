
#include "emt/emt.hpp"
#include "my_io_cb.hpp"
#include "thread_worker.hpp"


int main(int argc,char * argv[])
{
    int port =18600;
    unsigned workerCount =4;
    
    std::string errorstr;
    emt::EventLoop eventLoop(emt::EventLoop::GenBindInfo(port),workerCount);
    eventLoop.Init(my_readcb,my_notifycb,my_writecb,my_errorcb,my_timecb,my_acceptcb,errorstr);
    
    typedef std::tr1::shared_ptr < MyRunnable > T_MyRunnable;
    typedef std::tr1::shared_ptr < cf::Thread > T_Thread;
    std::vector < T_MyRunnable > runnables;
    std::vector < T_Thread > threads;
    for(unsigned i=0;i<workerCount;i++)
    {
        emt::T_Queue * queue =eventLoop.GenQueue(int(i), errorstr);
        emt::Notify * notify =eventLoop.GenNotify(int(i), errorstr);
        
        MyRunnable * myrun =new MyRunnable(queue,notify);
        T_MyRunnable run(myrun);
        runnables.push_back(run);
        
        T_Thread t(new cf::Thread(myrun,int(i)));
        threads.push_back(t);
    }
    
    for(std::vector < T_Thread >::iterator it =threads.begin();it!=threads.end();it++)
        (*it)->Start();


    int rt =eventLoop.Start(errorstr);
    if(-1==rt)
    {
        printf("Failed start event loop! %s \n",errorstr.c_str());
    }

    void *res;;
    for(std::vector < T_Thread >::iterator it =threads.begin();it!=threads.end();it++)
        (*it)->Join(&res);

    return 0;
}
