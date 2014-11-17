
#include "my_io_cb.hpp"
#include "est/est_util_network.hpp"
#include "est/est_libevent_eventloop.hpp"
#include "est/est_libevent_wrapper.hpp"
#include "cache_cmd.hpp"
#include "cache_cmd_arg.hpp"

est::EventLoop * my_eventLoop =NULL;

void my_readcb(bufferevent *bev, void *arg)
{
//    printf("my_readcb \n");
    if(NULL==arg||NULL==bev)
    {
        printf("my_readcb, NULL==arg||NULL==bev \n");
        return;
    }
    evutil_socket_t fd =bufferevent_getfd(bev);
    printf("my_readcb,fd=%d \n",fd);
    
    est::Session * session =(est::Session *)arg;
    
    evbuffer *input= bufferevent_get_input(bev);
    est::SessionBase::Status & status =session->_status;
    size_t input_len = evbuffer_get_length(input);  
    while(input_len>0)
    {
        if(status==est::SessionBase::STATUS_RECVING_HEADER)
        {
            if(input_len>=4)
            {
                std::string data(4,'\0');
                evbuffer_remove(input, &data[0], data.size());
                status =est::SessionBase::STATUS_RECVING_BODY;
                unsigned * u =(unsigned *)(&data[0]);
                unsigned size =*u;
                size =ntohl(size);
				if(size>1024000) // MAX PACKAGE SIZE, hard code temply.
				{
					return;
				}
                session->_bodySize =size;	    
				
                input_len = evbuffer_get_length(input);
                if(input_len<size)
                    break;
            }
            else
                break;
        }
        else
        {
            int size =session->_bodySize;
            if(int(input_len)>=size)
            {
                std::string data(size,'\0');
                evbuffer_remove(input, &data[0], data.size());
                
                cmd_step0(bev,data);
    				
                status =est::SessionBase::STATUS_RECVING_HEADER;
                input_len = evbuffer_get_length(input);
                if(input_len<4)
                    break;
            }
            else
                break;
        }
    }
}

void my_writecb(bufferevent *bev,void *arg)
{
//    printf("my_writecb \n");
    
    if(NULL==arg||NULL==bev)
    {
        printf("my_writecb, NULL==arg||NULL==bev \n");
        return;
    }
    evutil_socket_t fd =bufferevent_getfd(bev);
    printf("my_writecb,fd=%d \n",fd);
}

void my_errorcb(bufferevent *bev,short event,void *arg)
{
//    printf("my_errorcb \n");
    
    if(NULL==arg||NULL==bev)
    {
        printf("my_errorcb, NULL==arg||NULL==bev \n");
        return;
    }
    evutil_socket_t fd =bufferevent_getfd(bev);
    printf("my_errorcb,fd=%d \n",fd);
    
    int rt =my_eventLoop->RemoveSession(fd);
    if(0!=rt)
        ;// TODO: Warning , session not found when try to remove!
}

void my_timecb(int fd,short event,void *ctx)
{
    printf("my_timecb \n");
}


void my_acceptcb(evutil_socket_t listener, short event, void *arg)
{
    printf("my_acceptcb \n");
    unsigned remoteip =0;
    int remoteport =0;
    unsigned localip =0;
    int localport =0;
    int fd =est::Accept(listener,remoteip,remoteport,localip,localport);

    if(NULL==arg)
    {
        printf("my_acceptcb, NULL==arg");
        return;
    }
    est::AcceptcbArg * acceptcbArg =(est::AcceptcbArg *)arg;
    
    printf("specId=%d \n",acceptcbArg->specId);
    
    est::T_Session output_session;
    std::string errorstr;
    int rt =est::CreateSessionByFd(acceptcbArg->base,fd,acceptcbArg->specId,
                      my_readcb,my_writecb,my_errorcb,
                      output_session,errorstr);
    if(0!=rt) // error!
    {
        printf("Failed CreateSessionByFd, %s \n",errorstr.c_str());
    }
    my_eventLoop->InsertSession(fd,output_session);
}

