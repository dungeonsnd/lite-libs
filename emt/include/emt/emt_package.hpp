#ifndef _HEADER_FILE_EMT_PACKAGE_HPP_
#define _HEADER_FILE_EMT_PACKAGE_HPP_

#include "emt/emt_include_cpp.hpp"

namespace emt
{
// Package中的唯一连接检验码unique用来与Session对象中的unique进行比较.
//  工作线程中处理完数据准备发送时，从(网络线程与工作线程)共享数据区根据userid取出目标fd及unique(共享数据区需要加锁访问)。
//  这样投入到发送队列Notify中的Package都带了unique标识，网络线程从发送队列取出时把Package的unique与目标Session对象中的进行比较，
//  不同说明原连接已经关闭，并且fd被复用了，此Session不是目标fd对应的Session，不应该发送该Package了。
struct Package
{
    int _fd;
    int _unique;
    std::string _data;
};

} // namespace emt

#endif // _HEADER_FILE_EMT_PACKAGE_HPP_
