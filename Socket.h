
#ifndef __MMUDUO_NET_SOCKET_H__

#define __MMUDUO_NET_SOCKET_H__

#include <errno.h>
#include <iostream>

using namespace std;

namespace mmuduo
{

namespace net
{

class Socket
{
    private:
    const int m_socket;

    public:
    Socket();
    ~Socket();

    int getSocket() const {return m_socket;}
    void bind(struct sockaddr * server_addr);
    void listen();

    void shutdownWrite(int sockfd);

    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);
    void setNoBlock();

};

}
}

#endif //__MMUDUO_NET_SOCKET_H__