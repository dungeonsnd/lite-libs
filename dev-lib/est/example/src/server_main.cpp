
#include "est/est.hpp"
#include "my_io_cb.hpp"
#include "specid.hpp"

int main(int argc,char * argv[])
{
    
    est::EventLoop::T_BindInfo bindinfo;
    est::IpPort ipport;
    ipport.port =18600;    
    bindinfo[SPECID_FRONT_LISTEN] =ipport;
    ipport.port =18601;
    bindinfo[SPECID_MAINTAINANCE_LISTEN] =ipport;
    
    est::EventLoop eventLoop(bindinfo);
    my_eventLoop =&eventLoop;
    std::string errorstr;
    int rt =eventLoop.Start(my_acceptcb,errorstr);
    if(-1==rt)
    {
        printf("Failed start event loop! %s \n",errorstr.c_str());
    }
    return 0;
}

