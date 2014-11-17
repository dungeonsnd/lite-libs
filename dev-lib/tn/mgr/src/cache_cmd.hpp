#ifndef _HEADER_FILE_CACHE_CMD_HPP_
#define _HEADER_FILE_CACHE_CMD_HPP_

#include "cache_cmd_arg.hpp"
#include "cache_async.hpp"
#include <stdlib.h>
#include <string.h>

extern CacheAsync * my_cache;

inline void SetCacheAsync(CacheAsync * cache)
{
    my_cache =cache;
}


inline void cmd_step0_cb(redisAsyncContext *c, void *r, void *privdata)
{
    printf("--cmd_step0_cb-- \n");
    redisReply *reply = (redisReply *)r;
    if (reply == NULL) return;
    
    CacheCmdArg * cmdargs =(CacheCmdArg *)privdata;
    std::string data(reply->str); 
    unsigned t=data.size();
    std::string bd(t+4,'\0');
    t=htonl(t);
    memcpy(&bd[0],&t,sizeof(unsigned));
    memcpy(&bd[4],data.c_str(), data.size());
    
    printf("reply:%s \n",data.c_str());
    
    int rt =bufferevent_write(cmdargs->_bev,bd.c_str(), bd.size());
    delete cmdargs;
    if(0!=rt) // TODO:
    {    
    }
}


inline void cmd_step0(bufferevent * bev,const std::string & data) 
{
    printf("--cmd_step0-- \n");
    CacheCmdArg * cmdargs=new CacheCmdArg(bev,"");
    if(my_cache)
        my_cache->AsyncCommand(cmd_step0_cb, cmdargs, data.c_str());
    else
        printf("my_cache is NULL! \n");
}





#endif // _HEADER_FILE_CACHE_CMD_HPP_
