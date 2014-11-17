#ifndef _HEADER_FILE_EMT_PIPE_HPP_
#define _HEADER_FILE_EMT_PIPE_HPP_

#include "emt/emt_include_system.hpp"
#include "emt/emt_include_cpp.hpp"

namespace emt
{

class Pipe
{
public:
    int fd_r;
    int fd_w;
    Pipe()
    {
        int fds[2];
        int rt =pipe(fds);
        if(-1==rt)
            printf("Failed pipe,errno=%d,%s \n",errno,strerror(errno));
        fd_r =fds[0];
        fd_w =fds[1];
    }
    ~Pipe()
    {
        close(fd_r);
        close(fd_w);
    }
};


} // namespace emt


#endif // _HEADER_FILE_EMT_PIPE_HPP_
