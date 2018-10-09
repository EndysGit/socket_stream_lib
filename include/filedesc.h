#ifndef SOCKET_STREAM_LIB_FILEDESC_H
#define SOCKET_STREAM_LIB_FILEDESC_H

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <string>

#include "_filedesc_config.h"

/*********************
 * Global prefixes convention 
 *  c - constant
 *  s - static 
 * 
 * Local prefixes:
 *  fd - file descriptor
 *  
 */

_FILEDESC_NAMESPACE_BEGIN

#define BUF2V(x) static_cast<void *>(x)

namespace 
{
    const fd_buf_sz_t c_bufsz_1    = 1;
    const fd_buf_sz_t c_bufsz_2    = 2;
    const fd_buf_sz_t c_bufsz_4    = 4;  
    const fd_buf_sz_t c_bufsz_8    = 8; 
    const fd_buf_sz_t c_bufsz_16   = 16;
    const fd_buf_sz_t c_bufsz_32   = 32;
    const fd_buf_sz_t c_bufsz_64   = 64;  
    const fd_buf_sz_t c_bufsz_128  = 128;
    const fd_buf_sz_t c_bufsz_256  = 256;
    const fd_buf_sz_t c_bufsz_512  = 512;
    const fd_buf_sz_t c_bufsz_1024 = 1024;  
    const fd_buf_sz_t c_bufsz_2048 = 2048;
    const fd_buf_sz_t c_bufsz_4096 = 4096;
}

class fd_exception : public std::exception
{
public:
    fd_exception() noexcept;
    virtual const char * whaat() const noexcept;
    virtual ~fd_exception() noexcept; 
};

class Ifd_base
{
public:
    virtual Ifd_base& open(const std::string &path, flag_t flag, mode_t mode) = 0;
    virtual ssize_t read(base_buf_t buffer, fd_buf_sz_t size) = 0;
    virtual ssize_t write(base_buf_t buffer, fd_buf_sz_t size) = 0;
    virtual int close() = 0;
    virtual ~Ifd_base() = 0;
};

_FILEDESC_NAMESPACE_END


#endif //SOCKET_STREAM_LIB_FILEDESC_H
