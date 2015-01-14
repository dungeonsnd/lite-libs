
#include "my_io_cb.hpp"
#include "emt/emt_util_network.hpp"
#include "emt/emt_libevent_eventloop.hpp"

void my_notifycb(bufferevent *bev, void *arg)
{
//    printf("my_notifycb \n");
    
    if(NULL==arg||NULL==bev)
    {
        printf("my_notifycb, NULL==arg||NULL==bev \n");
        return;
    }
    evutil_socket_t fd =bufferevent_getfd(bev);
//    printf("my_notifycb,fd=%d \n",fd);
    
    
    emt::EventLoop * eventLoop =(emt::EventLoop *)arg;
    emt::Notify * notify =eventLoop->GetNotify(fd);
    if(NULL==notify)
    {
        printf("my_notifycb, NULL==notify \n");
        return;
    }
    
    struct evbuffer *input= bufferevent_get_input(bev);
    std::string data(4096,'\0');
    evbuffer_remove(input, &data[0], data.size());  // TODO: 这里应该清空，而不是取出至多4096。

    try
    {
        while(1)
        {
            
            emt::Package pkg =notify->Recv();
            int & fd =pkg._fd;
            int & unique =pkg._unique;
            std::string & data =pkg._data;
            
            emt::T_Session session;
            int rt =eventLoop->GetSession(fd,session);
            if(0!=rt)
            {
                printf("Failed GetSession \n");
                return;
            }
            
            if(unique!=session->_unique) // TODO: session has been closed. Discard this package.
            {
                printf("Failed GetSession \n");
                continue;
            }
            
            struct evbuffer *output =bufferevent_get_output(session->_bev);
            evbuffer_add(output, data.c_str(), data.size()); 
        };
    }catch (cf::QueueEmptyException & e) // empty now.
    {
    }
}

    
void my_readcb(bufferevent *bev, void *arg)
{
//    printf("my_readcb \n");
    if(NULL==arg||NULL==bev)
    {
        printf("my_readcb, NULL==arg||NULL==bev \n");
        return;
    }
    evutil_socket_t fd =bufferevent_getfd(bev);
//    printf("my_readcb,fd=%d \n",fd);
    
    emt::EventLoop * eventLoop =(emt::EventLoop *)arg;
    emt::EventLoop::T_Queues & queues =eventLoop->GetQueues();

    emt::T_Session session;
    int rt =eventLoop->GetSession(fd,session);
    if(0!=rt)
    {
        printf("Failed GetSession \n");
        return;
    }
    
    struct evbuffer *input= bufferevent_get_input(bev);
    emt::SessionBase::Status & status =session->_status;
    size_t input_len = evbuffer_get_length(input);  
    while(input_len>0)
    {
        if(status==emt::SessionBase::STATUS_RECVING_HEADER)
        {
            if(input_len>=4)
            {
                std::string data(4,'\0');
                evbuffer_remove(input, &data[0], data.size());
                status =emt::SessionBase::STATUS_RECVING_BODY;
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
                
                emt::Package pkg;
                pkg._fd =fd;
                pkg._unique =session->_unique;
                pkg._data =data;
                
                queues[fd%queues.size()]->Put(pkg);
				
                status =emt::SessionBase::STATUS_RECVING_HEADER;
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
//    evutil_socket_t fd =bufferevent_getfd(bev);
//    printf("my_writecb,fd=%d \n",fd);
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
//    printf("my_errorcb,fd=%d \n",fd);
    
    emt::EventLoop * eventLoop =(emt::EventLoop *)arg;
    
    int rt =eventLoop->RemoveSession(fd);
    if(0!=rt)
        ;// TODO: Warning , session not found when try to remove!
}

void my_timecb(int fd,short event,void *ctx)
{
//    printf("my_timecb \n");
}

void my_acceptcb(evutil_socket_t listener, short event, void *arg)
{
//    printf("my_acceptcb \n");
    unsigned remoteip =0;
    int remoteport =0;
    unsigned localip =0;
    int localport =0;
    int fd =emt::Accept(listener,remoteip,remoteport,localip,localport);

    if(NULL==arg)
    {
        printf("my_acceptcb, NULL==arg");
        return;
    }
    emt::EventLoop * eventLoop =(emt::EventLoop *)arg;
    
    emt::T_Session output_session;
    std::string errorstr;
    int rt =emt::CreateSessionByFd(eventLoop->GetBase(),fd,
                      my_readcb,my_writecb,my_errorcb,
                      eventLoop,output_session,errorstr);
    if(0!=rt) // error!
    {
        printf("Failed CreateSessionByFd, %s \n",errorstr.c_str());
    }
    eventLoop->InsertSession(fd,output_session);
}

