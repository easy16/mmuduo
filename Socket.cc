#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

#include "Socket.h"

using namespace mmuduo;
using namespace mmuduo::net;

#define MAX_LISTENFD 5

Socket::Socket():m_socket(::socket(AF_INET, SOCK_STREAM, 0))
{

    //FIXME CHECK
}

Socket::~Socket()
{
    close(m_socket);
}

void Socket::bind(struct sockaddr * server_addr)
{

    if(-1 == ::bind(m_socket, server_addr, sizeof(struct sockaddr)))
    {
        cout << "bind fail, errno:" << errno << endl; 
    }
}

void Socket::listen()
{
    int ret = ::listen(m_socket, MAX_LISTENFD);
    if (ret < 0)
    {
        cout << "sockets::listen fail" << endl;
    }
}

void Socket::shutdownWrite(int sockfd)
{
    if (::shutdown(sockfd, SHUT_WR) < 0)
    {
        cout << "sockets::shutdown fail" << endl;
    }
}

void Socket::setTcpNoDelay(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void Socket::setReuseAddr(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void Socket::setReusePort(bool on)
{
    
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEPORT,
                         &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0)
    {
        cout << "SO_REUSEPORT failed." << endl;
    }
#else
    if (on)
    {
        cout << "SO_REUSEPORT is not supported." << endl;
    }
#endif
}

void Socket::setKeepAlive(bool on)
{
    int optval = on ? 1 : 0;
    ::setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE,
               &optval, static_cast<socklen_t>(sizeof optval));
  // FIXME CHECK
}

void Socket::setNoBlock()
{
    int flags = fcntl(m_socket, F_GETFL, 0);
    fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);//no-block io

    //FIXME CHECK
}

