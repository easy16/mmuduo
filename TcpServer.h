#ifndef __MMUDUO_NET_TCPSERVER_H__

#define __MMUDUO_NET_TCPSERVER_H__


namespace mmuduo
{

namespace net
{

class Socket;

class TcpServer
{
public:
	TcpServer();
	~TcpServer();

	void start();

private:
	//mmuduo::net::Socket sock;
	
};

}
}

#endif //__MMUDUO_NET_TCPSERVER_H__