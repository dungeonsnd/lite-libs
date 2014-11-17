#ifndef _HEADER_FILE_CACHE_CMD_ARG_HPP_
#define _HEADER_FILE_CACHE_CMD_ARG_HPP_

#include "include_libevent_async.hpp"
#include <string>

class CacheCmdArg
{
public:
    bufferevent * _bev;
    std::string _data;
    
    CacheCmdArg(bufferevent * bev,const std::string & data):
        _bev(bev),_data(data)
    {
    }
};


#endif // _HEADER_FILE_CACHE_CMD_ARG_HPP_
