/*

echo server unsing epoll

*/

#include "Socket.h"
#include "TcpServer.h"

using namespace mmuduo;
using namespace mmuduo::net;


int main(int argc, char** argv)
{
    TcpServer tcpserver;
    tcpserver.start();

    return 0;
}