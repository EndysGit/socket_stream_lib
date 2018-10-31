#include "socket.h"

using namespace filedesc;
using namespace filedesc::network;
ssize_t
socket_base::write(base_buf_t buffer, fd_buf_sz_t size)
{
    return 0;
}

ssize_t
socket_base::read(base_buf_t buffer, fd_buf_sz_t size)
{
    return 0;
}

Ifd_base*
socket_base::open(const std::string &path, flag_t flag, mode_t mode)
{
    socket_base *sock = new socket_base();
    return sock; 
}

int 
socket_base::close() noexcept
{
    return 0;
}

socket_base::~socket_base() noexcept 
{}
