#include "socket.h"

_FD_NETWORK_NAMESPACE_BEGIN

socket_base::sock_state
socket_base::open(socket_base::address_family_t domain,
     socket_base::sock_type_t type, 
     socket_base::protocol_family_t protocol)
{
    m_domain   = domain;
    m_type     = type;
    m_protocol = protocol;

    m_c_socket = socket(m_domain, m_type, m_protocol);

    if (m_c_socket != 0)
    {
        m_state = invalid;
        throw socket_exception("Failed to open the socket.");
    }

    m_state = valid | unused;

    return socket_base::valid;
}

ssize_t
socket_base::write(base_buf_t buffer, fd_buf_sz_t size)
{
    if (m_state != invalid)
    {
        ::write(m_c_socket, buffer, size);
    }
    else 
    {
        throw socket_exception("Attempt to write to invalid scoket.");
    }
    return 0;
}

ssize_t
socket_base::read(base_buf_t buffer, fd_buf_sz_t size)
{
    if (m_state != invalid)
    {
       ::read(m_c_socket, buffer, size);
    }
    else 
    {
        throw socket_exception("Attempt to read from invalid socket.");
    }
    return 0;
}

void
socket_base::close() 
{
    if (::close(m_c_socket) != 0)
    {
        m_state = invalid;

        throw socket_exception("Failed to close the socket");
    }
}

bool
socket_base::is_valid()
{
    return static_cast<bool>(m_state & valid);    
}

bool 
socket_base::is_used()
{
    return static_cast<bool>(m_state & used);
}

void 
socket_base::set_state(sock_state state)
{

}
_FD_NETWORK_NAMESPACE_END
