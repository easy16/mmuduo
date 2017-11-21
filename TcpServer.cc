
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>

#include "Socket.h"
#include "TcpServer.h"

using namespace std;

using namespace mmuduo;
using namespace mmuduo::net;

#define MAX_EVENTS 1000
#define MAX_LINE 1024

TcpServer::TcpServer()
{

}

TcpServer:: ~TcpServer()
{

}

void TcpServer::start()
{
	struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    int listenfd;
    int connfd;
    int sockfd;
    int read_size;

    char line[MAX_LINE];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(struct sockaddr_in);

    int epollfd = epoll_create(1);
    if (epollfd < 0)
    {
        std::cout << "epoll_ create fail";
        exit(-1);
    }
    
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(11111);

    mmuduo::net::Socket sock;
    sock.setNoBlock();
    sock.setReuseAddr(1);
    sock.bind((struct sockaddr *) &server_addr);
    sock.listen();

    listenfd = sock.getSocket();

    ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

    for(; ;)
    {
        int fds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
        if ( fds == -1)
        {
            std:: cout << "epoll_wait fail" << std::endl;
            break;
        }
        cout << "loop epoll_wait return fds: " << fds << endl; 
        for (int i = 0; i < fds; i ++)
        {
            if (events[i].data.fd == listenfd)
            {
                connfd = accept(listenfd, (sockaddr*)&client_addr, (socklen_t*)&client_len);
                if (connfd > 0)
                {
                    cout << "new connection from " 
                        << "[" << inet_ntoa(client_addr.sin_addr) 
                        << ":" << ntohs(client_addr.sin_port) << "]" 
                        << " accept socket fd: " << connfd 
                        << endl;
                }
                else
                {
                    cout << "accept fail, connfd: " << connfd << endl;
                }
                fcntl(connfd, F_SETFL, O_NONBLOCK); //no-block io
                ev.data.fd = connfd;
                ev.events = EPOLLIN;
                if (-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev))
                {
                    cout << "epoll_ctl fail in for loop" << endl;
                }
            }
            else if(events[i].events & EPOLLIN)
            {
                sockfd = events[i].data.fd;
                if(sockfd < 0)
                {
                    cout << "sockfd fail, sockfd: " << sockfd << endl;
                }
                bzero(line, MAX_LINE);
                read_size = read(sockfd, line, MAX_LINE);
                if(read_size < 0)
                {
                    if(errno == ECONNRESET)
                    {
                        cout << "ECONNREST closed socket fd: " << events[i].data.fd << endl;
                        close(sockfd);
                    } 
                }
                else if (read_size == 0)
                {
                    cout << "read 0 from socket: " << events[i].data.fd << " close it!"<< endl;
                    close(events[i].data.fd);
                }
                else
                {
                    cout << "read content : " << line << "from: "
                        << "[" << inet_ntoa(client_addr.sin_addr) 
                        << ":" << ntohs(client_addr.sin_port) << "] "
                        << "sockfd : " << sockfd << endl;
                    if(write(sockfd, line, read_size) != read_size)
                    {
                        cout << "echo data error: not finished one time" << endl;
                    }
                }
            }           
        }
    }
}