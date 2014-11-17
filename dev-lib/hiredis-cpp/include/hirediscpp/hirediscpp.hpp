/*
 * Copyright 2013, Jeffery Qiu. All rights reserved.
 *
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.gnu.org/licenses/lgpl.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//// Author: Jeffery Qiu (dungeonsnd at gmail dot com)
////

#ifndef _HEADER_FILE_HIREDISCPP_HPP_
#define _HEADER_FILE_HIREDISCPP_HPP_

#include <hiredis.h>

namespace hr
{

class ReplyGuard
{
public:
    ReplyGuard(redisReply * reply):_reply(reply)
    {}
    void Reset(redisReply * reply)
    {
        freeReplyObject(_reply);
        _reply =reply;
    }
    ~ReplyGuard()
    {
        freeReplyObject(_reply);
    }
    redisReply * Reply()
    {
        return _reply;
    }
private:
    redisReply * _reply;
};

class RedisConn
{
public:
    RedisConn(const char * host,unsigned port,unsigned timeoutSec,unsigned tryConnectTimes=3):
        _host(host),_port(port),_timeoutSec(timeoutSec),_connected(false),_tryConnectTimes(tryConnectTimes)
    {
    }
    
    int Connect()
    {
        int ret =0;
        int n=0;
        struct timeval timeout ={timeoutSec,0};
        while(++n<=_tryConnectTimes)
        {
            _c = redisConnectWithTimeout(_host.c_str(), _port, _timeoutSec);
            if (_c == NULL || _c->err)
            {
                if (_c)
                {
                    printf("Connection error: %s\n", _c->errstr);
                    redisFree(_c);
                    ret =1;
                }
                else
                {
                    printf("Connection error: can't allocate redis context\n");
                    ret =2;
                }
            }
            if(0==ret)
            {
                _connected =true;
                break;
            }
        }
        return ret;
    }
    
    redisReply * execCommand(const char * cmd)
    {
        redisReply * reply = (redisReply *)redisCommand(_c,cmd);
        printf("SET: %s\n", reply->str);
        return reply;
    }
    ~RedisConn()
    {
        redisFree(_c);
    }
private:
    redisContext * _c;
    std::string _host;
    unsigned _port;
    unsigned _timeoutSec;
    bool _connected;
    unsigned _tryConnectTimes;
};

} // namespace hr

#endif // _HEADER_FILE_HIREDISCPP_HPP_

